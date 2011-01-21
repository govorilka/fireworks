#ifndef FIREWORKS_GRAPHICSSTRINGITEM_INL_H
#define FIREWORKS_GRAPHICSSTRINGITEM_INL_H

#include "fwprimitives/fwstringprimitive.h"

QString FwGraphicsStringItem::string() const
{
    return m_string;
}

FwFont FwGraphicsStringItem::font() const
{
    return m_font;
}

bool FwGraphicsStringItem::isShadowEnable() const
{
    return m_shadow;
}

FwColor FwGraphicsStringItem::shadowColor() const
{
    return m_shadowColor;
}

bool FwGraphicsStringItem::isFixedSize() const
{
    return m_fixedSize;
}


#endif // FIREWORKS_GRAPHICSSTRINGITEM_INL_H
