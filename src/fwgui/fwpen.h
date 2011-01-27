#ifndef FIREWORKS_PEN_H
#define FIREWORKS_PEN_H

#include <QtCore/qline.h>
#include <QtCore/qrect.h>
#include <QtCore/qpoint.h>
#include <QtCore/qbytearray.h>

#include "fwcore/fwcolor.h"

class FwPainter;

class FwPen
{
public:
    FwPen(int width, const FwColor& color);

    inline int width() const;
    inline FwColor color() const;

    void drawLine(FwPainter* painter, const QLine& line);
    void drawString(FwPainter* painter, const QPoint& pos, const QByteArray& utf8String);

    inline QRect clientArea(const QRect& rect) const;

    void drawStroke(FwPainter* painter, const QRect& rect);

private:
    int m_width;
    FwColor m_color;
};

int FwPen::width() const
{
    return m_width;
}

FwColor FwPen::color() const
{
    return m_color;
}

QRect FwPen::clientArea(const QRect& rect) const
{
    return rect.adjusted(m_width, m_width, -m_width, -m_width);
}

#endif // FWPEN_H
