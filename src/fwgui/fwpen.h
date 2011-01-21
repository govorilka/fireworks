#ifndef FIREWORKS_PEN_H
#define FIREWORKS_PEN_H

#include <QtCore/qline.h>
#include <QtCore/qrect.h>
#include <QtCore/qsharedpointer.h>
#include <QtCore/qpoint.h>
#include <QtCore/qbytearray.h>

#include "fwcore/fwcolor.h"

class FwCanvas;

class FwPen
{
public:
    FwPen(int width, const FwColor& color);

    inline int width() const;
    inline FwColor color() const;

    void drawLine(FwCanvas* canvas, const QLine& line);
    QRect drawStroke(FwCanvas* canvas, const QRect& rect);
    void drawString(FwCanvas* canvas, const QPoint& pos, const QByteArray& utf8String);

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

////////////////////////////////////////////////////////////////////////////////

typedef QSharedPointer<FwPen> FwPenPtr;

#endif // FWPEN_H
