#ifndef FIREWORKS_GRAPHICSRECTITEM_H
#define FIREWORKS_GRAPHICSRECTITEM_H

#include <QtGui/qcolor.h>

#include "fwgui/fwbrush.h"
#include "fwgui/fwpixmap.h"

#include "fwprimitives/fwprimitive.h"

class FwBrush;

class FwRectPrimitive : public FwPrimitive
{
    typedef FwPrimitive BaseClass;

public:
    FwRectPrimitive(FwPrimitiveGroup* parent);
    ~FwRectPrimitive();

    inline FwBrushPtr brush() const;
    void setBrush(FwBrushPtr brush);

    virtual void apply(FwMLObject *object);

protected:
    QRect updateGeometry(const QRect& rect);

    void paint(FwCanvas* canvas);
    void paint(FwCanvas* canvas, const QRect& rect);

private:
    FwBrushPtr m_brush;
};

FwBrushPtr FwRectPrimitive::brush() const
{
    return m_brush;
}

#endif // FIREWORKS_GRAPHICSRECTITEM_H
