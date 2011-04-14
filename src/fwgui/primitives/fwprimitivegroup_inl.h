#ifndef FIREWORKS_GRAPHICSITEMGROUP_INL_H
#define FIREWORKS_GRAPHICSITEMGROUP_INL_H

#include "fwgui/primitives/fwprimitivegroup.h"

QVector<FwPrimitive*> FwPrimitiveGroup::primitives() const
{
    return m_primitives;
}

void FwPrimitiveGroup::sortZIndex()
{
    needSortZIndex = true;
    updateChildren();
}

void FwPrimitiveGroup::updateChildren()
{
    FwPrimitiveGroup* parent = this;
    while(parent)
    {
        if(parent->childrenDirty)
        {
            break;
        }
        parent->childrenDirty = true;
        parent = parent->m_parent;
    }
}

void FwPrimitiveGroup::updateChildrenRect()
{
    childrenRectDirty = true;
    updateChildren();
}

#endif // FIREWORKS_GRAPHICSITEMGROUP_INL_H
