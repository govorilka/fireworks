#ifndef FIREWORKS_DRAWER_H
#define FIREWORKS_DRAWER_H

#include <QtCore/qrect.h>

class FwPrimitive;
class FwPainter;

class FwDrawer
{
public:
    FwDrawer(const FwPrimitive* primitive);

    inline const FwPrimitive* primitive() const;

    virtual void paint(FwPainter* painter, const QRect& clipRect) = 0;

private:
    const FwPrimitive* m_primitive;
};

const FwPrimitive* FwDrawer::primitive() const
{
    return m_primitive;
}

///////////////////////////////////////////////////////////////////

class FwRoundedRectDrawer : public FwDrawer
{
    typedef FwDrawer BaseClass;
public:
    FwRoundedRectDrawer(int radius, const FwPrimitive* primitive);

    inline int radius() const;

    void paint(FwPainter* painter, const QRect& clipRect);

    void roundedRectMidpoint(FwPainter* painter, const QRect& rect, int radius);
    void circlePlotPoints(FwPainter* painter, const QPoint& circPt);

private:
    int m_ax;
    int m_ay;
    int m_bx;
    int m_by;
    int m_radius;
};

int FwRoundedRectDrawer::radius() const
{
    return m_radius;
}

#endif // FIREWORKS_DRAWER_H
