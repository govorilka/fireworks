#ifndef FIREWORKS_DESCRIPTIONS_H
#define FIREWORKS_DESCRIPTIONS_H

#include <string>

namespace Fw
{
    enum VideoModeType
    {
        VMT_Interlace,
        VMT_Progressive
    };

    typedef unsigned char UChar;

    class DescRect;
    class DescColor;
    class DescPixmap;
    class DescFont;
    class DescKey;
    class DescResolution;
}

////////////////////////////////////////////////////////////////////////////////

class Fw::DescRect
{
public:
    DescRect();

    int x;
    int y;
    int width;
    int height;
};

////////////////////////////////////////////////////////////////////////////////

class Fw::DescColor
{
public:
    DescColor();

    UChar red;
    UChar green;
    UChar blue;
    UChar alpha;

    std::string name;
};

////////////////////////////////////////////////////////////////////////////////

class Fw::DescPixmap
{
public:
    DescPixmap();

    bool blending;
    std::string source;
};

////////////////////////////////////////////////////////////////////////////////

class Fw::DescFont
{
public:
    DescFont();

    std::string family;
    int pixelSize;
};

////////////////////////////////////////////////////////////////////////////////

class Fw::DescKey
{
public:
    DescKey();

    int code;
    char ascii;
    int repeatCount;
};

////////////////////////////////////////////////////////////////////////////////

class Fw::DescResolution
{
public:
    DescResolution();

    std::string name;
    int width;
    int height;
    int hz;
    int bpp;
    VideoModeType modeType;
};


#endif // FIREWORKS_DESCRIPTIONS_H
