#ifndef FIREWORKS_QPRENDER_H
#define FIREWORKS_QPRENDER_H

#include <QtGui/qpainter.h>

#include "fwgui/fwrender.h"

class QPaintDevice;

class QPRender : public FwRender
{
    typedef FwRender BaseClass;

public:
    QPRender(QPaintDevice* device);

protected:

    void setClipRect(const QRect& rect);

    void setColor(const FwColor& color);
    void setOpacity(float opacity);

    void setFont(FwFontData* font);
    void resetFont();

    void drawRect(const QRect& rect);
    void drawFillRect(const QRect& rect);
    void drawLine(const QLine& line);
    void drawPixmap(int x, int y, FwPixmapData* pixmap);
    void drawPixmap(const QRect& rect, FwPixmapData* pixmap, const QRect* srcRect);
    void drawString(int x, int y, const QByteArray& utf8String);

private:
    QPainter m_painter;
    QColor m_color;
};

#endif // FIREWORKS_QPRENDER_H
