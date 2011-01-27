#ifndef FIREWORKS_GRAPHICSITEMGROUP_INL_H
#define FIREWORKS_GRAPHICSITEMGROUP_INL_H

#include "fwprimitives/fwprimitivegroup.h"

QVector<FwPrimitive*> FwPrimitiveGroup::primitives() const
{
    return m_primitives;
}

void FwPrimitiveGroup::sortZIndex()
{
    if(m_primitives.size() > 1)
    {
        qSort(m_primitives.begin(), m_primitives.end(), FwPrimitive::zIndexLessThan);
    }
}

#endif // FIREWORKS_GRAPHICSITEMGROUP_INL_H
