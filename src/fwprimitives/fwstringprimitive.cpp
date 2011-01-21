#include <QtCore/qdebug.h>

#include "fwstringprimitive.h"

#include "fwgui/fwcanvas.h"
#include "fwgui/fwscene.h"

FwGraphicsStringItem::FwGraphicsStringItem(FwPrimitiveGroup* parent) :
    BaseClass(parent),
    m_textPos(0, 0),
    m_shadow(false),
    m_shadowColor(0x00, 0x00, 0x00, 0x66), //40% black
    m_fixedSize(false)
{
}

void FwGraphicsStringItem::setString(const QString& string)
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

void FwGraphicsStringItem::setFont(const FwFont& font)
{
    if(m_font != font)
    {
        prepareGeometryChanged();
        m_font = font;
        if(m_fixedSize && !m_mask.isEmpty())
        {
            setSize(stringSize(m_mask));
        }
        update();
    }
}

/*!
Устанавливает или отключает отображение тени для текста
\param enable Если true, то для текста будет отображаться тень
\sa void StringPrimitive::setShadowEnabled(bool enable)
*/
void FwGraphicsStringItem::setShadowEnabled(bool enable)
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
void FwGraphicsStringItem::setShadowColor(const FwColor &color)
{
    if(m_shadowColor != color)
    {
        m_shadowColor = color;
        invalidate();
    }
}

QRect FwGraphicsStringItem::updateGeometry(const QRect& rect)
{
    m_textPos = rect.topLeft();
    m_textPos.setY(m_textPos.y() + m_font.ascender());
    return rect;
}

void FwGraphicsStringItem::paint(FwCanvas* canvas)
{
    canvas->setFont(m_font);

    if(m_shadow)
    {
        canvas->setColor(m_shadowColor);
        canvas->drawUtf8String(m_textPos.x() + 2, m_textPos.y() + 2, m_utf8String);
    }

    FwPenPtr pen = this->pen();
    if(pen)
    {
        pen->drawString(canvas, m_textPos, m_utf8String);
    }
}

void FwGraphicsStringItem::updatePointer(FwGraphicsStringItemPtr& string,
                                    const FwFont& font,
                                    FwPrimitiveGroup* parent)
{
    if(font.isNull())
    {
        if(string)
        {
            delete string;
            string = 0;
        }
    }
    else
    {
        if(!string)
        {
            string = new FwGraphicsStringItem(parent);
        }
        string->setFont(font);
    }
}

QSize FwGraphicsStringItem::stringSize(const QString& string) const
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

void FwGraphicsStringItem::setFixedSize(bool enable, const QString& mask)
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
