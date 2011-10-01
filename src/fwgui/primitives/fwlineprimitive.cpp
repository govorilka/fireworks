#include "fwlineprimitive.h"

#include "fwgui/fwpen.h"
#include "fwgui/fwpainter.h"

#include "fwcore/fwml.h"

FwLinePrimitive::FwLinePrimitive(const QByteArray& name, FwPrimitiveGroup* parent) :
    BaseClass(name, parent),
    m_lenght(0),
    m_orientation(0)
{
}

void FwLinePrimitive::setLine(const QLine& line)
{
    if(line != this->line())
    {
        prepareGeometryChanged();
        setPos(line.p1());

        m_p2 = line.p2() - line.p1();

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

void FwLinePrimitive::boundingRectChangedEvent(QRect& boundingRect)
{
    FwPen* pen = this->pen();
    if(pen && m_lenght)
    {
        switch(m_orientation)
        {
        case Fw::O_Horizontal:
            boundingRect.setSize(QSize(m_lenght, pen->width()));
            break;

        case Fw::O_Vertical:
            boundingRect.setSize(QSize(pen->width(), m_lenght));
            break;

        case Fw::O_Diagonal:
            boundingRect.setSize(QSize(pen->width() + m_p2.x(), pen->width() + m_p2.y()));
            break;

        default:
            break;
        }
    }
    else
    {
        boundingRect.setSize(QSize(0, 0));
    }
}

void FwLinePrimitive::paint(FwPainter *painter, const QRect &clipRect)
{
    Q_UNUSED(clipRect);

    FwPen* pen = this->pen();
    if(m_lenght && pen)
    {
        pen->drawLine(painter, QLine(geometry()->pos(), m_p2 + geometry()->pos()));
    }
}

void FwLinePrimitive::apply(FwMLObject *object)
{
    prepareGeometryChanged();

    bool bOk = false;

    int x1 = 0;
    FwMLNode* x1Node= object->attribute("x1");
    if(x1Node)
    {
        x1 = x1Node->toInt(&bOk);
    }

    int y1 = 0;
    FwMLNode* y1Node= object->attribute("y1");
    if(y1Node)
    {
        y1 = y1Node->toInt(&bOk);
    }

    int x2 = 0;
    FwMLNode* x2Node= object->attribute("x2");
    if(x2Node)
    {
        x2 = x2Node->toInt(&bOk);
    }

    int y2 = 0;
    FwMLNode* y2Node= object->attribute("y2");
    if(y2Node)
    {
        y2 = y2Node->toInt(&bOk);
    }

    setLine(x1, y1, x2, y2);

    BaseClass::apply(object);

    update();
}

