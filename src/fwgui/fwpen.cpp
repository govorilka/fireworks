#include "fwpen.h"
#include "fwpainter.h"

FwPen::FwPen(int width, const FwColor& color) :
    m_width(width),
    m_color(color)
{
}

void FwPen::drawLine(FwPainter* painter, const QLine& line)
{
    painter->setColor(m_color);

    if(!line.dx())
    {
        painter->drawFillRect(QRect(line.p1(), QSize(m_width, line.dy())));
    }
    else if(!line.dy())
    {
        painter->drawFillRect(QRect(line.p1(), QSize(line.dx(), m_width)));
    }
    else
    {
        painter->drawLine(line); //TODO!!!
    }
}

void FwPen::drawStroke(FwPainter* painter, const QRect& rect)
{
    painter->setColor(m_color);
    QRect r = rect;
    for(int i = 0; i < m_width; i++)
    {
        painter->drawRect(r);
        r.adjust(1, 1, -1, -1);
    }
}

void FwPen::drawString(FwPainter* painter, const QPoint& pos, const QByteArray& utf8String)
{
    painter->setColor(m_color);
    painter->drawString(pos.x(), pos.y(), utf8String);
}
