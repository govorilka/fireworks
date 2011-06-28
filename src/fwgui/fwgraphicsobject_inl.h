#ifndef FIREWORKS_GRAPHICSOBJECT_INL_H
#define FIREWORKS_GRAPHICSOBJECT_INL_H

#include "fwgui/fwgraphicsobject.h"

FwFont FwGraphicsObject::font() const
{
    return m_font;
}

QPropertyAnimation* FwGraphicsObject::showAnimation() const
{
    return m_showAnimation;
}

QPropertyAnimation* FwGraphicsObject::hideAnimation() const
{
    return m_hideAnimation;
}

#endif // FIREWORKS_GRAPHICSOBJECT_INL_H
