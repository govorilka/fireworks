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

    virtual void setColor(const FwColor& color) = 0;
    virtual void setFont(const FwFont& font) = 0;

    virtual void drawFillRect(int x, int y, int w, int h) = 0;

    virtual void prepareDrawSurface(FwPixmapData* surface);
    virtual void drawPixmap(int x, int y, FwPixmapData* pixmap) = 0;
    virtual void drawPixmap(const QRect& rect, FwPixmapData* pixmap, const QRect* srcRect) = 0;

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

    inline int startX() const;
    inline int startY() const;

protected:
    int m_startX;
    int m_startY;
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
    restoreClip(rect.isNull() ? rect : rect.translated(m_startX, m_startY));
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

int FwRender::startX() const
{
    return m_startX;
}

int FwRender::startY() const
{
    return m_startY;
}

#endif //FIREWORKS_RENDER_H
