#ifndef FIREWORKS_GRAPHICSLINEITEM_INL_H
#define FIREWORKS_GRAPHICSLINEITEM_INL_H

#include "fwprimitives/fwlineprimitive.h"

FwPenPtr FwLinePrimitive::pen() const
{
    return m_pen;
}

void FwLinePrimitive::setPen(const FwPenPtr& pen)
{
    if(m_pen != pen)
    {
        m_pen = pen;
        invalidate();
    }
}

QLine FwLinePrimitive::line() const
{
    return QLine(xy(), m_p2);
}

void FwLinePrimitive::setLine(int x1, int y1, int x2, int y2)
{
    setLine(QLine(x1, y1, x2, y2));
}

int FwLinePrimitive::orientation() const
{
    return m_orientation;
}

int FwLinePrimitive::length() const
{
    return m_lenght;
}

#endif //FIREWORKS_GRAPHICSLINEITEM_INL_H
