#ifndef FIREWORKS_GRAPHICSITEMGROUP_INL_H
#define FIREWORKS_GRAPHICSITEMGROUP_INL_H

#include "fwprimitives/fwprimitivegroup.h"

QVector<FwPrimitive*> FwPrimitiveGroup::items() const
{
    return m_items;
}

void FwPrimitiveGroup::sortZIndex()
{
    if(m_items.size() > 1)
    {
        qSort(m_items.begin(),m_items.end(), FwPrimitive::zIndexLessThan);
    }
}

#endif // FIREWORKS_GRAPHICSITEMGROUP_INL_H
