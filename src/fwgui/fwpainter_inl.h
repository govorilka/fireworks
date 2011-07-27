#ifndef FIREWORKS_PAINTER_INL_H
#define FIREWORKS_PAINTER_INL_H

#include "fwgui/fwpainter.h"

int FwPainter::nullX() const
{
    return m_nullX;
}

int FwPainter::nullY() const
{
    return m_nullY;
}

QRect FwPainter::window() const
{
    return QRect(m_nullX, m_nullY, m_windowSize.width(), m_windowSize.height());
}

QRect FwPainter::clipRect() const
{
    return m_clipRect;
}

QRect FwPainter::setClipRect(const QRect& rect)
{
    QRect oldRect = m_clipRect;
    restoreClipRect(rect.translated(-m_nullX, -m_nullY));
    return oldRect;
}

void FwPainter::restoreClipRect(const QRect& rect)
{
    if(m_clipRect != rect)
    {
        m_render->setClipRect(m_clipRect = rect);
    }
}

FwColor FwPainter::color() const
{
    return m_color;
}

void FwPainter::setColor(quint8 a, quint8 r, quint8 g, quint8 b)
{
    setColor(FwColor(a, r, g, b));
}

void FwPainter::setColor(const FwColor& color)
{
    if(m_color != color)
    {
        m_color = color;
        m_render->setColor(color);
    }
}

float FwPainter::opacity() const
{
    return m_opacity;
}

void FwPainter::setOpacity(float opacity)
{
    if(m_opacity != opacity)
    {
        m_opacity = opacity;
        m_render->setOpacity(opacity);
    }
}

FwFont FwPainter::font() const
{
    return m_font;
}

void FwPainter::setFont(const FwFont& font)
{
    if(m_font != font)
    {
        m_font = font;
        if(m_font.isNull())
        {
            m_render->resetFont();
        }
        else
        {
            m_render->setFont(m_font.data());
        }
    }
}

void FwPainter::drawFillRect(const QRect& rect)
{
    m_render->drawFillRect(rect.translated(m_nullX, m_nullY));
}

void FwPainter::drawRect(const QRect& rect)
{
    m_render->drawRect(rect.translated(m_nullX, m_nullY));
}

void FwPainter::drawLine(const QLine& line)
{
    m_render->drawLine(line.translated(m_nullX, m_nullY));
}

void FwPainter::drawPixmap(int x, int y, const FwPixmap& pixmap)
{
    if(!pixmap.isNull())
    {
        m_render->drawPixmap(m_nullX + x, m_nullY + y, pixmap.surface());
    }
}

void FwPainter::drawPixmap(const QRect& rect, const FwPixmap& pixmap, const QRect* srcRect)
{
    if(srcRect && rect.size() == pixmap.size() && (*srcRect).size() == rect.size())
    {
        drawPixmap(rect.x(), rect.y(), pixmap);
    }
    else if(!pixmap.isNull())
    {
        m_render->drawPixmap(rect.translated(m_nullX, m_nullY), pixmap.surface(), srcRect);
    }
}

void FwPainter::drawString(int x, int y, const QByteArray& utf8String)
{
    m_render->drawString(m_nullX + x, m_nullY + y, utf8String);
}

void FwPainter::drawBuffer(const QRect& rect, FwPixmapData* pixmap, const QRect srcRect)
{
    m_render->drawPixmap(rect, pixmap, &srcRect);
}

void FwPainter::drawVertexArray(FwVertexArray* array)
{
    m_render->drawVertexArray(array);
}

#endif // FIREWORKS_PAINTER_INL_H
