#ifndef FIREWORKS_GRAPHICSSTRINGITEM_INL_H
#define FIREWORKS_GRAPHICSSTRINGITEM_INL_H

#include "fwprimitives/fwstringprimitive.h"

QString FwStringPrimitive::string() const
{
    return m_string;
}

FwFont FwStringPrimitive::font() const
{
    return m_font;
}

bool FwStringPrimitive::isShadowEnable() const
{
    return m_shadow;
}

FwColor FwStringPrimitive::shadowColor() const
{
    return m_shadowColor;
}

bool FwStringPrimitive::isFixedSize() const
{
    return m_fixedSize;
}


#endif // FIREWORKS_GRAPHICSSTRINGITEM_INL_H
