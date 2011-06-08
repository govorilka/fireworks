#ifndef FIREWORKS_COLOR_H
#define FIREWORKS_COLOR_H

#include <QtCore/qglobal.h>
#include <QtCore/qbytearray.h>

#include "fireworks.h"

class FwMLNode;

class FIREWORKSSHARED_EXPORT FwColor
{
public:
    FwColor();
    FwColor(quint32 argb);
    FwColor(quint8 a, quint8 r, quint8 g, quint8 b);

    inline quint8 red() const;
    inline void setRed(quint8 red);
    inline quint8 green() const;
    inline void setGreen(quint8 green);
    inline quint8 blue() const;
    inline void setBlue(quint8 blue);
    inline quint8 alpha() const;
    inline void setAlpha(quint8 alpha);

    inline quint32 argb() const;

    inline bool blending() const;
    inline void setBlending(bool blending);

    inline bool isNull() const;

    static quint32 nameToRGBA(const QByteArray& name, bool* bOk);

    inline bool operator!=(const FwColor& rhs) const;
    inline bool operator==(const FwColor& rhs) const;

    inline FwColor& operator=(const FwColor& rhs);

private:

    union {
        struct {
            quint8 m_alpha;
            quint8 m_red;
            quint8 m_green;
            quint8 m_blue;
        };
        quint32 m_allChannel;
    };

    bool m_blending;
};

quint8 FwColor::red() const
{
    return m_red;
}

void FwColor::setRed(quint8 red)
{
    m_red = red;
}

quint8 FwColor::green() const
{
    return m_green;
}

void FwColor::setGreen(quint8 green)
{
    m_green = green;
}

quint8 FwColor::blue() const
{
    return m_blue;
}

void FwColor::setBlue(quint8 blue)
{
    m_blue = blue;
}

quint8 FwColor::alpha() const
{
    return m_alpha;
}

void FwColor::setAlpha(quint8 alpha)
{
    m_alpha = alpha;
}

quint32 FwColor::argb() const
{
    return ((m_alpha & 0xff) << 24) |
           ((m_red & 0xff) << 16) |
           ((m_green & 0xff) << 8) |
            (m_blue & 0xff);
}

bool FwColor::blending() const
{
    return m_alpha != 255 ? m_blending : false;
}

void FwColor::setBlending(bool blending)
{
    m_blending = blending;
}

bool FwColor::isNull() const
{
    return m_alpha == 0 && m_blending == true;
}

bool FwColor::operator!=(const FwColor& rhs) const
{
    return m_allChannel != rhs.m_allChannel || m_blending != rhs.m_blending;
}

bool FwColor::operator==(const FwColor& rhs) const
{
    return m_allChannel == rhs.m_allChannel && m_blending == rhs.m_blending;
}

FwColor& FwColor::operator=(const FwColor& rhs)
{
    if(this != &rhs)
    {
        m_allChannel = rhs.m_allChannel;
        m_blending = rhs.m_blending;
    }
    return *this;
}

#endif // FIREWORKS_COLOR_H
