#ifndef FIREWORKS_RENDER_H
#define FIREWORKS_RENDER_H

#include <QtCore/qobject.h>
#include <QtCore/qrect.h>
#include <QtCore/qpoint.h>

#include "fwcore/fwcolor.h"

class FwFont;
class FwPixmapData;
class GuiShell;

class FwCanvas;

class FwRender
{
public:
    friend class FwCanvas;
    friend class GuiShell;

    explicit FwRender();
    virtual ~FwRender();

protected:
    virtual void setColor(const FwColor& color) = 0;
    virtual void setFont(const FwFont& font) = 0;

    virtual void drawFillRect(int x, int y, int w, int h) = 0;

    virtual void prepareDrawSurface(FwPixmapData* surface);
    virtual void drawSurface(int x, int y, FwPixmapData* surface, const QRect* srcRect) = 0;
    virtual void drawSurface(const QRect& rect, FwPixmapData* surface, const QRect* srcRect) = 0;

    virtual void drawRect(int x, int y, int w, int h) = 0;

    virtual void drawLine(int x1, int y1, int x2, int y2) = 0;

    virtual void flip(int x, int y, int w, int h) = 0;

    virtual void drawString(int x, int y, const QString& string) = 0;
    virtual void drawString(int x, int y, const char* str, int strSize) = 0;

    inline QRect saveClip() const;
    inline QRect setClip(const QRect& rect);
    inline void restoreClip(const QRect& rect);

    virtual void updateClipRect(const QRect& rect) = 0;

    virtual void clear() = 0;

    inline void updateColor();

    int startX;
    int startY;
    QPoint nullPos;
    QRect rect;
    qreal opacity;

private:   
    QRect m_clipRect;
    FwColor colorBuffer;
};

QRect FwRender::saveClip() const
{
    return m_clipRect;
}

QRect FwRender::setClip(const QRect& rect)
{
    QRect oldRect = m_clipRect;
    restoreClip(rect.isNull() ? rect : rect.translated(startX, startY));
    return oldRect;
}

void FwRender::restoreClip(const QRect& rect)
{
    if(m_clipRect != rect)
    {
        m_clipRect = rect;
        updateClipRect(m_clipRect);
    }
}

void FwRender::updateColor()
{
    FwColor displayColor = colorBuffer;
    displayColor.setAlpha(displayColor.alpha() * opacity);
    setColor(displayColor);
}

#endif //FIREWORKS_RENDER_H
