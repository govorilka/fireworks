#ifndef FIREWORKS_GRAPHICSITEMGROUP_INL_H
#define FIREWORKS_GRAPHICSITEMGROUP_INL_H

#include "fwgui/primitives/fwprimitivegroup.h"

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

void FwPrimitiveGroup::updateChildRect()
{
    FwPrimitiveGroup* parent = this;
    while(parent)
    {
        parent->m_childrenRectDirty = true;
        parent = parent->m_parent;
    }
}

#endif // FIREWORKS_GRAPHICSITEMGROUP_INL_H