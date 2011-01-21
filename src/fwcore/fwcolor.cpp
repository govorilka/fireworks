#include "fwcolor.h"
#include "fwml.h"

FwColor::FwColor() :
    m_allChannel(0),
    m_blending(true)
{
}

FwColor::FwColor(quint32 rgba) :
    m_red((rgba >> 24) & 0xFF),
    m_green((rgba >> 16) & 0xFF),
    m_blue((rgba >> 8) & 0xFF),
    m_alpha(rgba & 0xFF),
    m_blending(m_alpha != 255)
{
}

FwColor::FwColor(quint8 r, quint8 g, quint8 b, quint8 a) :
    m_red(r),
    m_green(g),
    m_blue(b),
    m_alpha(a),
    m_blending(m_alpha != 255)
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
            return 0x00000000;
        }
        else if(nameLower == "black")
        {
            (*bOk) = true;
            return 0x000000FF;
        }
        else if(nameLower == "white")
        {
            (*bOk) = true;
            return 0xFFFFFFFF;
        }
    }

    (*bOk) = false;
    return 0;
}
