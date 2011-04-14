#include <QtCore/qdebug.h>

#include "fwstringprimitive.h"

#include "fwcore/fwml.h"

#include "fwgui/fwscene.h"
#include "fwgui/fwgraphicsview.h"
#include "fwgui/fwpainter.h"

FwStringPrimitive::FwStringPrimitive(const QByteArray& name, FwPrimitiveGroup* parent) :
    BaseClass(name, parent),
    m_textPos(0, 0),
    m_fixedSize(false),
    m_stringRect(0, 0, 0, 0)
{
}

void FwStringPrimitive::setString(const QString& string)
{
    if(m_string != string)
    {
        prepareGeometryChanged();
        m_string = string;
        m_utf8String = string.toUtf8();
        if(!m_fixedSize)
        {
            updateStringRect(m_string);
        }
        update();
    }
}

void FwStringPrimitive::setFont(const FwFont& font)
{
    if(m_font != font)
    {
        prepareGeometryChanged();
        m_font = font;
        updateStringRect(m_fixedSize && !m_mask.isEmpty() ? m_mask : m_string);
        update();
    }
}

void FwStringPrimitive::boundingRectChangedEvent(QRect &boundingRect)
{
    boundingRect = m_stringRect.translated(boundingRect.topLeft());
}

void FwStringPrimitive::geometryChangedEvent(const QRect &oldRect, QRect &rect)
{
    Q_UNUSED(oldRect);
    m_textPos = rect.topLeft();
    m_textPos.setY(m_textPos.y() + m_font.ascender());
}

void FwStringPrimitive::paint(FwPainter *painter, const QRect &clipRect)
{
    Q_UNUSED(clipRect);
    if(pen())
    {
        painter->setFont(m_font);
        pen()->drawString(painter, m_textPos, m_utf8String);
    }
}

void FwStringPrimitive::updateStringRect(const QString &string)
{
    m_stringRect = pen() ? pen()->stringRect(m_font, string) : QRect(0, 0, 0, 0);
    setSize(m_stringRect.size() - QSize(m_stringRect.x(), m_stringRect.y()));
}

void FwStringPrimitive::setFixedSize(bool enable, const QString& mask)
{
    if(m_fixedSize != enable)
    {
        m_fixedSize = enable;
        if(m_fixedSize)
        {
            if(!mask.isEmpty())
            {
                m_mask = mask;
                updateStringRect(m_mask);
            }
        }
        else
        {
            updateStringRect(m_string);
        }
    }
}

void FwStringPrimitive::apply(FwMLObject *object)
{
    prepareGeometryChanged();

    FwFont font = createFont(object, "font");
    if(!font.isNull())
    {
        setFont(font);
    }

    FwMLString* stringNode = object->attribute("string")->cast<FwMLString>();
    if(stringNode)
    {
        setString(QString::fromUtf8(stringNode->value()));
    }

    BaseClass::apply(object);

    update();
}

void FwStringPrimitive::penChangedEvent(FwPen* pen)
{
    updateStringRect(m_fixedSize && !m_mask.isEmpty() ? m_mask : m_string);
}
