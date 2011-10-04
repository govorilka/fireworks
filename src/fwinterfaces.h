#ifndef FIREWORKS_INTERFACES_H
#define FIREWORKS_INTERFACES_H

#include "fwdescriptions.h"

namespace Fw
{
    enum DisplayLayerID
    {
        Layer_OSD,
        Layer_Player
    };

    class IResourceLibrary;
    class IRender;
    class IDisplay;
    class IDisplayLayer;

    typedef void(*CallbackInput)(const DescKey& key);
    typedef void(*CallbackDraw)(IRender* render, const DescRect& rect);
    typedef void(*CallbackResolution)(const DescResolution& resolution);
}

////////////////////////////////////////////////////////////////////////////////

class Fw::IResourceLibrary
{
public:
    IResourceLibrary();
    virtual ~IResourceLibrary() = 0;

    virtual bool loadColor(const DescColor& color, int* id) = 0;
    virtual bool loadPixmap(const DescColor& color, int* id) = 0;
    virtual bool loadFont(const DescColor& color, int* id) = 0;
};

////////////////////////////////////////////////////////////////////////////////

class Fw::IRender
{
public:
    IRender();
    virtual ~IRender() = 0;

    virtual void setColor(int colorID) = 0;
    virtual void setColor(UChar red, UChar green, UChar blue, UChar alpha) = 0;
    virtual void setOpacity(float opacity) = 0;

    virtual void setFont(int fontID) = 0;

    virtual void drawFillRect(const DescRect& rect) = 0;
    virtual void drawPixmap(int x, int y, int pixmapID) = 0;
    virtual void drawPixmap(const DescRect& rect, int pixmapID, const DescRect& srcRect) = 0;
    virtual void drawString(int x, int y, const std::string& utf8String) = 0;
};

////////////////////////////////////////////////////////////////////////////////

class Fw::IDisplay
{
public:
    IDisplay();
    virtual ~IDisplay() = 0;

    virtual void setInputCallback(CallbackInput callback) = 0;

    virtual DescResolution resolution() const = 0;
    virtual bool setResolution(const DescResolution& resolution) = 0;
    virtual void setResolutionCallback(CallbackResolution callback) = 0;

    virtual bool setStandBy(bool enable) = 0;

    virtual IResourceLibrary* resourceLibrary() const = 0;
    virtual IDisplayLayer* displayLayer(int id) const = 0;
};

////////////////////////////////////////////////////////////////////////////////


class Fw::IDisplayLayer
{
public:
    IDisplayLayer();
    virtual ~IDisplayLayer() = 0;

    virtual void setDrawCallback(CallbackDraw callback) = 0;

    virtual float opacity() const = 0;
    virtual void setOpacity(float opacity) = 0;



};

#endif // FIREWORKS_INTERFACES_H
