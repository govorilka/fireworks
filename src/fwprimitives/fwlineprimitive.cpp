#include "fwlineprimitive.h"

#include "fwgui/fwcanvas.h"

FwLinePrimitive::FwLinePrimitive(FwPrimitiveGroup* parent) :
    BaseClass(parent),
    m_lenght(0),
    m_orientation(0)
{
}

void FwLinePrimitive::setLine(const QLine& line)
{
    if(line != this->line())
    {
        prepareGeometryChanged();
        setXY(line.p1());
        m_p2 = line.p2();
        if(line.y1() == line.y2())
        {
            m_orientation = Fw::O_Horizontal;
            m_lenght = m_p2.x() - xy().x();

        }
        else if(line.x1() == line.x2())
        {
            m_orientation = Fw::O_Vertical;
            m_lenght = m_p2.y() - xy().y();
        }
        else
        {
            m_orientation = Fw::O_Diagonal;
            m_lenght = qRound(QLineF(line).length());
        }
        update();
    }
}

QRect FwLinePrimitive::updateGeometry(const QRect& rect)
{
    if(m_lenght)
    {
        QRect r = rect;
        switch(m_orientation)
        {
        case Fw::O_Horizontal:
            r.setSize(QSize(m_lenght, m_pen->width()));
            break;

        case Fw::O_Vertical:
            r.setSize(QSize(m_pen->width(), m_lenght));
            break;

        case Fw::O_Diagonal:
            r.setSize(QSize(m_pen->width() + m_p2.x(), m_pen->width() + m_p2.y()));
            break;

        default:
            break;
        }
        return r;
    }
    return QRect();
}

void FwLinePrimitive::paint(FwCanvas* canvas)
{
    if(m_lenght && m_pen)
    {
        m_pen->drawLine(canvas, line());
    }
}
