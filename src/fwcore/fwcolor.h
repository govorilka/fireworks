#ifndef FIREWORKS_COLOR_H
#define FIREWORKS_COLOR_H

#include <QtCore/qglobal.h>
#include <QtCore/qbytearray.h>

class FwMLNode;

class FwColor
{
public:
    FwColor();
    FwColor(quint32 rgba);
    FwColor(quint8 r, quint8 g, quint8 b, quint8 a);

    inline quint8 red() const;
    inline void setRed(quint8 red);
    inline quint8 green() const;
    inline void setGreen(quint8 green);
    inline quint8 blue() const;
    inline void setBlue(quint8 blue);
    inline quint8 alpha() const;
    inline void setAlpha(quint8 alpha);

    inline bool blending() const;
    inline void setBlending(bool blending);

    inline bool isNull() const;

    static quint32 nameToRGBA(const QByteArray& name, bool* bOk);

    inline bool operator!=(const FwColor& rhs) const;
    inline bool operator==(const FwColor& rhs) const;

private:

    union {
        struct {
            quint8 m_red;
            quint8 m_green;
            quint8 m_blue;
            quint8 m_alpha;
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
    m_alpha =alpha;
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

#endif // FIREWORKS_COLOR_H
