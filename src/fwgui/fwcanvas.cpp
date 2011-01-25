#include <QtCore/qdebug.h>

#include "fwcanvas.h"
#include "fwpixmap.h"

FwCanvas::FwCanvas() :
    m_render(0)
{
}

FwCanvas::FwCanvas(FwRender* r) :
    m_render(r)
{
    m_render->updateColor();
}

FwCanvas::FwCanvas(const FwCanvas& rhs) :
    m_render(rhs.m_render)
{
}

FwCanvas::~FwCanvas()
{
}

QPoint FwCanvas::startPos() const
{
    return QPoint(m_render->startX(), m_render->startY());
}

QRect FwCanvas::rect() const
{
    return m_render->rect;
}

void FwCanvas::setRect(const QRect& rect)
{
    if(m_render->rect != rect)
    {
        m_render->rect = rect;
        updateStartPos();
    }
}

QPoint FwCanvas::nullPos() const
{
    return m_render->nullPos;
}

void FwCanvas::setNullPos(const QPoint& nullPos)
{
    if(m_render->nullPos != nullPos)
    {
        m_render->nullPos = nullPos;
        updateStartPos();
    }
}

void FwCanvas::setColor(const FwColor& color)
{
    if(m_render->colorBuffer != color)
    {
        m_render->colorBuffer = color;
        m_render->updateColor();
    }
}

void FwCanvas::setFont(const FwFont& font)
{
    m_render->setFont(font);
}

void FwCanvas::drawFillRect(const QRect& rect)
{
    m_render->drawFillRect(m_render->startX() + rect.x(),
                         m_render->startY() + rect.y(),
                         rect.width(),
                         rect.height());
}

void FwCanvas::drawFillRect(const FwColor& color, const QRect& rect)
{
    FwColor colorBuffer = m_render->colorBuffer;
    if(m_render->colorBuffer != color)
    {
        setColor(color);
        m_render->drawFillRect(m_render->startX() + rect.x(),
                             m_render->startY() + rect.y(),
                             rect.width(),
                             rect.height());
        setColor(colorBuffer);
    }
    else
    {
        m_render->drawFillRect(m_render->startX() + rect.x(),
                             m_render->startY() + rect.y(),
                             rect.width(),
                             rect.height());
    }
}

void FwCanvas::drawRect(const QRect& rect)
{
    m_render->drawRect(m_render->startX() + rect.x(),
                     m_render->startY() + rect.y(),
                     rect.width(),
                     rect.height());
}

void FwCanvas::drawString(int x, int y, const QString& string)
{
    m_render->drawString(m_render->startX() + x,
                       m_render->startY() + y,
                       string);
}

void FwCanvas::drawUtf8String(int x, int y, const QByteArray& utf8String)
{
    m_render->drawString(m_render->startX() + x,
                       m_render->startY() + y,
                       utf8String.data(),
                       utf8String.size());
}

void FwCanvas::drawLine(const QLine& line)
{
    m_render->drawLine(line.x1(), line.y1(), line.x2(), line.y2());
}

void FwCanvas::flip(const QRect& rect)
{
    m_render->flip(m_render->startX() + rect.x(),
                 m_render->startY() + rect.y(),
                 rect.width(),
                 rect.height());
}

void FwCanvas::updateStartPos()
{
    m_render->m_startX = m_render->nullPos.x() - m_render->rect.x();
    m_render->m_startY = m_render->nullPos.y() - m_render->rect.y();
}

void FwCanvas::setOpacity(qreal opacity)
{
    if(m_render->opacity != opacity)
    {
        m_render->opacity = opacity;
        m_render->updateColor();
    }
}

void FwCanvas::clear()
{
    m_render->clear();
}
