#ifndef FIREWORKS_RENDER_H
#define FIREWORKS_RENDER_H

#include <QtCore/qobject.h>
#include <QtCore/qrect.h>
#include <QtCore/qline.h>
#include <QtCore/qpoint.h>
#include <QtCore/qbytearray.h>

#include "fwcore/fwcolor.h"

class FwFontData;
class FwPixmapData;

class FwVertexArray
{
public:
    explicit FwVertexArray();
    virtual ~FwVertexArray();

    virtual void begin() = 0;
    virtual void end() = 0;

    virtual void addLine(int x1, int y1, int x2, int y2) = 0;
    virtual void addRect(int x, int y, int w, int h) = 0;
};

/////////////////////////////////////////////////////////////////////////////////////

class FwRender
{
public:
    explicit FwRender();
    virtual ~FwRender();

    virtual void setClipRect(const QRect& rect) = 0;

    virtual void setColor(const FwColor& color) = 0;
    virtual void setOpacity(float opacity) = 0;

    virtual void setFont(FwFontData* font) = 0;
    virtual void resetFont() = 0;

    virtual void drawRect(const QRect& rect) = 0;
    virtual void drawFillRect(const QRect& rect) = 0;
    virtual void drawLine(const QLine& line) = 0;
    virtual void drawPixmap(int x, int y, FwPixmapData* pixmap) = 0;
    virtual void drawPixmap(const QRect& rect, FwPixmapData* pixmap, const QRect* srcRect) = 0;
    virtual void drawString(int x, int y, const QByteArray& utf8String) = 0;
    virtual void drawVertexArray(FwVertexArray* array) = 0;
};

#endif //FIREWORKS_RENDER_H
