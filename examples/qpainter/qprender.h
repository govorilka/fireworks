#ifndef FIREWORKS_QPRENDER_H
#define FIREWORKS_QPRENDER_H

#include <QtGui/qpainter.h>

#include "fwgui/fwrender.h"

#include"fw_backend_qpainter.h"

class QPRender;
class QPaintDevice;

class FW_BACKEND_QPAINTERSHARED_EXPORT QPVertexArray : public FwVertexArray
{
    typedef FwVertexArray BaseClass;

public:
    friend class QPRender;

    explicit QPVertexArray();
    ~QPVertexArray();

    void begin();
    void end();

    void addLine(int x1, int y1, int x2, int y2);
    void addRect(int x, int y, int w, int h);

private:
    QVector<QLine> m_lines;
    QVector<QRect> m_rectangles;
};

/////////////////////////////////////////////////////////////////////////////////////

class FW_BACKEND_QPAINTERSHARED_EXPORT QPRender : public FwRender
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
    void drawVertexArray(FwVertexArray* array);

private:
    QPainter m_painter;
    QColor m_color;
};

#endif // FIREWORKS_QPRENDER_H
