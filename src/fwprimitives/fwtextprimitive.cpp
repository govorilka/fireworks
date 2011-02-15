#include <QtCore/qdebug.h>

#include "fwtextprimitive.h"

#include "fwgui/fwpainter.h"
#include "fwgui/fwscene.h"
#include "fwgui/fwgraphicsview.h"
#include "fwgui/fwpen.h"

#include "fwcore/fwml.h"

#include "fwtypography/fwjustification.h"

FwTextPrimitive::FwTextPrimitive(const QByteArray& name, FwPrimitiveGroup* parent) :
    BaseClass(name, parent),
    m_pen(new FwPen(1, FwColor(0xFFFFFFFF)))
{
}

FwTextPrimitive::~FwTextPrimitive()
{
    delete m_pen;
}

void FwTextPrimitive::setText(const QString& text)
{
    if(m_text != text)
    {
        prepareGeometryChanged();
        m_text = text;
        update();
    }
}

void FwTextPrimitive::setFont(const FwFont& font)
{
    if(m_font != font)
    {
       prepareGeometryChanged();
       m_font = font;
       update();
    }
}

void FwTextPrimitive::setPen(FwPen* pen)
{
    if(m_pen != pen)
    {
        delete m_pen;
        m_pen = pen;
        invalidate();
    }
}

void FwTextPrimitive::updateGeometry(const QRect &rect, QRect& boundingRect)
{
    if(m_text.isEmpty())
    {
        m_strings = QVector<FwTextString>();
    }
    else
    {
        FwJustification jEngine;
        jEngine.setBlockRect(rect);
        jEngine.setFont(m_font);
        m_strings = jEngine.processing(m_text);
    }
    BaseClass::updateGeometry(rect, boundingRect);
    boundingRect = rect;
}

void FwTextPrimitive::paint(FwPainter *painter, const QRect &clipRect)
{
    BaseClass::paint(painter, clipRect);

    if(!m_strings.isEmpty() && m_pen)
    {
        painter->setColor(m_pen->color());
        painter->setFont(m_font);

        FwTextString* string = m_strings.data();
        for(int i = 0; i < m_strings.count(); i++, string++)
        {
            painter->drawString(string->x(), string->y(), string->utf8());
        }
    }
}

void FwTextPrimitive::apply(FwMLObject *object)
{
    prepareGeometryChanged();

    FwPen* pen = createPen(object, "pen");
    if(pen)
    {
         setPen(pen);
    }

    FwFont font = createFont(object, "font");
    if(!font.isNull())
    {
        setFont(font);
    }

    BaseClass::apply(object);

    update();
}
