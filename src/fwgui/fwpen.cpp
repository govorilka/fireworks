#include "fwpen.h"
#include "fwcanvas.h"

FwPen::FwPen(int width, const FwColor& color) :
    m_width(width),
    m_color(color)
{
}

void FwPen::drawLine(FwCanvas* canvas, const QLine& line)
{
    canvas->setColor(m_color);
    if(!line.dx())
    {
        canvas->drawFillRect(QRect(line.p1(), QSize(m_width, line.dy())));
    }
    else if(!line.dy())
    {
        canvas->drawFillRect(QRect(line.p1(), QSize(line.dx(), m_width)));
    }
    else
    {
        canvas->drawLine(line); //TODO!!!
    }
}

QRect FwPen::drawStroke(FwCanvas* canvas, const QRect& rect)
{
    canvas->setColor(m_color);
    QRect r = rect;
    for(int i = 0; i < m_width; i++)
    {
        canvas->drawRect(r);
        r.adjust(1, 1, -1, -1);
    }
    return r;
}

void FwPen::drawString(FwCanvas* canvas, const QPoint& pos, const QByteArray& utf8String)
{
    canvas->setColor(m_color);
    canvas->drawUtf8String(pos.x(), pos.y(), utf8String);
}
