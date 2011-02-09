#ifndef FIREWORKS_PEN_H
#define FIREWORKS_PEN_H

#include <QtCore/qline.h>
#include <QtCore/qrect.h>
#include <QtCore/qpoint.h>
#include <QtCore/qbytearray.h>

#include "fwcore/fwcolor.h"

#include "fwtypography/fwfont.h"

class FwPainter;

class FwShadow
{
public:
    FwShadow(const QPoint& pos, const FwColor& color);

    inline QPoint pos() const;
    inline FwColor color() const;

private:
    QPoint m_pos;
    FwColor m_color;
};

QPoint FwShadow::pos() const
{
    return m_pos;
}

FwColor FwShadow::color() const
{
    return m_color;
}

////////////////////////////////////////////////////////////////////////////////

class FwPen
{
public:
    FwPen(int width, const FwColor& color);
    ~FwPen();

    inline int width() const;
    inline FwColor color() const;

    inline FwShadow* shadow() const;
    void setShadow(FwShadow* shadow);

    void drawLine(FwPainter* painter, const QLine& line);
    void drawString(FwPainter* painter, const QPoint& pos, const QByteArray& utf8String);

    inline QRect clientArea(const QRect& rect) const;

    void drawStroke(FwPainter* painter, const QRect& rect);

    QRect stringRect(const FwFont& font, const QString& string);

private:
    int m_width;
    FwColor m_color;
    FwShadow* m_shadow;
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

FwShadow* FwPen::shadow() const
{
    return m_shadow;
}

#endif // FWPEN_H
