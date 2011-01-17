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

    union
    {
        struct
        {
            quint8 red;
            quint8 green;
            quint8 blue;
            quint8 alpha;
        };
        quint32 rgba;
    };

    bool blending;

    inline bool needBlending() const;

    inline bool isNull() const;

    static quint32 nameToRGBA(const QByteArray& name, bool* bOk);

    inline bool operator!=(const FwColor& rhs) const;
    inline bool operator==(const FwColor& rhs) const;
};

bool FwColor::needBlending() const
{
    return alpha != 255 ? blending : false;
}

bool FwColor::isNull() const
{
    return alpha == 0 && blending == true;
}

bool FwColor::operator!=(const FwColor& rhs) const
{
    return alpha != rhs.alpha || blending != rhs.blending;
}

bool FwColor::operator==(const FwColor& rhs) const
{
    return alpha == rhs.alpha && blending == rhs.blending;
}

#endif // FIREWORKS_COLOR_H
