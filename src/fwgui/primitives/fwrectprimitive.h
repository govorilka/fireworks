#ifndef FIREWORKS_GRAPHICSRECTITEM_H
#define FIREWORKS_GRAPHICSRECTITEM_H

#include <QtGui/qcolor.h>

#include "fwgui/fwbrush.h"
#include "fwgui/fwpixmap.h"

#include "fwgui/primitives/fwprimitive.h"

class FwBrush;

class FwRectPrimitive : public FwPrimitive
{
    typedef FwPrimitive BaseClass;

public:
    FwRectPrimitive(const QByteArray& name, FwPrimitiveGroup* parent);
    ~FwRectPrimitive();

    inline FwBrush* brush() const;
    void setBrush(FwBrush* brush);

    virtual void apply(FwMLObject *object);

protected:
    void geometryChanged(const QRect &oldRect, QRect &rect);

    void paint(FwPainter* painter, const QRect& clipRect);

private:
    FwBrush* m_brush;
};

FwBrush* FwRectPrimitive::brush() const
{
    return m_brush;
}

#endif // FIREWORKS_GRAPHICSRECTITEM_H