#include "fwcolor.h"
#include "fwml.h"

FwColor::FwColor() :
   rgba(0),
   blending(true)
{
}

FwColor::FwColor(quint32 rgba)
{
    this->rgba = rgba;
    this->blending = (alpha != 255);
}

FwColor::FwColor(quint8 r, quint8 g, quint8 b, quint8 a) :
    red(r),
    green(g),
    blue(b),
    alpha(a),
    blending(alpha != 255)
{
}

quint32 FwColor::nameToRGBA(const QByteArray& name, bool* bOk)
{
    if(!name.isEmpty())
    {
        QByteArray nameLower = name.toLower();
        if(nameLower == "transparent")
        {
            (*bOk) = true;
            return 0;
        }
    }

    (*bOk) = false;
    return 0;
}
