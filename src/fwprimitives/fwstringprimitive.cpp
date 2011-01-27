#include <QtCore/qdebug.h>

#include "fwstringprimitive.h"

#include "fwcore/fwml.h"

#include "fwgui/fwscene.h"
#include "fwgui/fwgraphicsview.h"
#include "fwgui/fwpainter.h"

FwStringPrimitive::FwStringPrimitive(const QByteArray& name, FwPrimitiveGroup* parent) :
    BaseClass(name, parent),
    m_textPos(0, 0),
    m_shadow(false),
    m_shadowColor(0x00, 0x00, 0x00, 0x66), //40% black
    m_fixedSize(false)
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
            setSize(stringSize(m_string));
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
        if(m_fixedSize && !m_mask.isEmpty())
        {
            setSize(stringSize(m_mask));
        }
        else
        {
            setSize(stringSize(m_string));
        }
        update();
    }
}

/*!
Устанавливает или отключает отображение тени для текста
\param enable Если true, то для текста будет отображаться тень
\sa void StringPrimitive::setShadowEnabled(bool enable)
*/
void FwStringPrimitive::setShadowEnabled(bool enable)
{
    if(m_shadow != enable)
    {
        prepareGeometryChanged();
        m_shadow = enable;
        update();
    }
}

/*!
Устанавливает цвет тени для текста
\param color Цвет тени
\note По умолчанию для тени установлен цвет 0x00000066 (40% черный).
\sa void StringPrimitive::setShadowColor(const FwColor& color)
*/
void FwStringPrimitive::setShadowColor(const FwColor &color)
{
    if(m_shadowColor != color)
    {
        m_shadowColor = color;
        invalidate();
    }
}

QRect FwStringPrimitive::updateGeometry(const QRect& rect)
{
    m_textPos = rect.topLeft();
    m_textPos.setY(m_textPos.y() + m_font.ascender());
    return rect;
}

void FwStringPrimitive::paint(FwPainter *painter, const QRect &clipRect)
{
    painter->setFont(m_font);

    if(m_shadow)
    {
        painter->setColor(m_shadowColor);
        painter->drawString(m_textPos.x() + 2, m_textPos.y() + 2, m_utf8String);
    }

    FwPen* pen = this->pen();
    if(pen)
    {
        pen->drawString(painter, m_textPos, m_utf8String);
    }
}

QSize FwStringPrimitive::stringSize(const QString& string) const
{
    QSize size = m_font.stringSize(string);
    size.setHeight(m_font.height());
    if(m_shadow)
    {
        size.setWidth(size.width() + 2);
        size.setHeight(size.height() + 2);
    }
    return size;
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
                setSize(stringSize(m_mask));
            }
        }
        else
        {
            setSize(stringSize(m_string));
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
