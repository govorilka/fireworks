#ifndef FIREWORKS_PIXMAPBAR_PRIMITIVE_H
#define FIREWORKS_PIXMAPBAR_PRIMITIVE_H

#include "fwgui/primitives/fwrectprimitive.h"

class FwPainter;

class FwPixmapBarPrimtive : public FwRectPrimitive
{
    typedef FwRectPrimitive BaseClass;

public:
    FwPixmapBarPrimtive(const QByteArray& name, FwPrimitiveGroup* parent);

    void apply(FwMLObject *object);

    inline FwPixmap leftPixmap() const;
    void setLeftPixmap(const FwPixmap& value);

    inline FwPixmap centerPixmap() const;
    void setCenterPixmap(const FwPixmap& value);

    inline FwPixmap rightPixmap() const;
    void setRightPixmap(const FwPixmap& value);

protected:
    void paint(FwPainter *painter, const QRect &clipRect);

private:
    int m_leftWidth;
    FwPixmap m_leftPixmap;

    int m_centerWidth;
    FwPixmap m_centerPixmap;

    int m_rightWidth;
    FwPixmap m_rightPixmap;
};

FwPixmap FwPixmapBarPrimtive::leftPixmap() const
{
    return m_leftPixmap;
}

FwPixmap FwPixmapBarPrimtive::centerPixmap() const
{
    return m_centerPixmap;
}

FwPixmap FwPixmapBarPrimtive::rightPixmap() const
{
    return m_rightPixmap;
}


#endif //FIREWORKS_PIXMAPBAR_PRIMITIVE_H
