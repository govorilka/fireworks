#ifndef FIREWORKS_GRAPHICSSTRINGITEM_INL_H
#define FIREWORKS_GRAPHICSSTRINGITEM_INL_H

#include "fwgui/primitives/fwstringprimitive.h"

QString FwStringPrimitive::string() const
{
    return m_string;
}

FwFont FwStringPrimitive::font() const
{
    return m_font;
}

bool FwStringPrimitive::isFixedSize() const
{
    return m_fixedSize;
}

#endif // FIREWORKS_GRAPHICSSTRINGITEM_INL_H
