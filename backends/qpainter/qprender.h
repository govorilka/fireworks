#ifndef FIREWORKS_QPRENDER_H
#define FIREWORKS_QPRENDER_H

#include <QtGui/qpainter.h>

#include "fwgui/fwrender.h"

class QPWidget;

class QPRender : public FwRender
{
    typedef FwRender BaseClass;

public:
    QPRender(QPWidget* w);

protected:
    void setColor(const FwColor& color);
    void setFont(const FwFont& font);

    void drawFillRect(int x, int y, int w, int h);

    void prepareDrawSurface(FwPixmapData* surface);
    void drawSurface(int x, int y, FwPixmapData* surface, const QRect* srcRect);
    void drawSurface(const QRect& rect, FwPixmapData* surface, const QRect* srcRect);

    void drawRect(int x, int y, int w, int h);

    void drawLine(int x1, int y1, int x2, int y2);

    void flip(int x, int y, int w, int h);

    void drawString(int x, int y, const QString& string);
    void drawString(int x, int y, const char* str, int strSize);

    void updateClipRect(const QRect& rect);

    void clear();

private:
    QPainter m_painter;
    QColor m_color;
};

#endif // FIREWORKS_QPRENDER_H
