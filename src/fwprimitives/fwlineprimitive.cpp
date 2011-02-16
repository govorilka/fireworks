#include "fwlineprimitive.h"

#include "fwgui/fwpen.h"
#include "fwgui/fwpainter.h"

FwLinePrimitive::FwLinePrimitive(const QByteArray& name, FwPrimitiveGroup* parent) :
    BaseClass(name, parent),
    m_lenght(0),
    m_orientation(0),
    m_pen(0)
{
}

FwLinePrimitive::~FwLinePrimitive()
{
    if(m_pen)
    {
        delete m_pen;
    }
}

void FwLinePrimitive::setPen(FwPen* pen)
{
    if(m_pen != pen)
    {
        prepareGeometryChanged();
        delete m_pen;
        m_pen = pen;
        penChangedEvent(pen);
        update();
    }
}

void FwLinePrimitive::setLine(const QLine& line)
{
    if(line != this->line())
    {
        prepareGeometryChanged();
        setPos(line.p1());
        m_p2 = line.p2();
        if(line.y1() == line.y2())
        {
            m_orientation = Fw::O_Horizontal;
            m_lenght = m_p2.x() - pos().x();

        }
        else if(line.x1() == line.x2())
        {
            m_orientation = Fw::O_Vertical;
            m_lenght = m_p2.y() - pos().y();
        }
        else
        {
            m_orientation = Fw::O_Diagonal;
            m_lenght = qRound(QLineF(line).length());
        }
        update();
    }
}

void FwLinePrimitive::geometryChanged(const QRect& oldRect, QRect& rect)
{
    Q_UNUSED(oldRect);
    if(m_lenght)
    {
        switch(m_orientation)
        {
        case Fw::O_Horizontal:
            rect.setSize(QSize(m_lenght, m_pen->width()));
            break;

        case Fw::O_Vertical:
            rect.setSize(QSize(m_pen->width(), m_lenght));
            break;

        case Fw::O_Diagonal:
            rect.setSize(QSize(m_pen->width() + m_p2.x(), m_pen->width() + m_p2.y()));
            break;

        default:
            break;
        }
    }
    else
    {
        rect.setSize(QSize(0, 0));
    }
}

void FwLinePrimitive::paint(FwPainter *painter, const QRect &clipRect)
{
    if(m_lenght && m_pen)
    {
        m_pen->drawLine(painter, line());
    }
}

void FwLinePrimitive::apply(FwMLObject *object)
{
    prepareGeometryChanged();

    FwPen* pen = createPen(object, "pen");
    if(pen)
    {
         setPen(pen);
    }

    BaseClass::apply(object);

    update();
}

void FwLinePrimitive::penChangedEvent(FwPen* pen)
{
    Q_UNUSED(pen);
}
