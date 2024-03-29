#ifndef FIREWORKS_GRAPHICSRECTITEM_H
#define FIREWORKS_GRAPHICSRECTITEM_H

#include "fwgui/fwbrush.h"
#include "fwgui/fwpixmap.h"

#include "fwgui/primitives/fwprimitive.h"

class FwBrush;

class FIREWORKSSHARED_EXPORT FwRectPrimitive : public FwPrimitive
{
    typedef FwPrimitive BaseClass;

public:
    FwRectPrimitive(const QByteArray& name, FwPrimitiveGroup* parent);
    ~FwRectPrimitive();

    inline FwBrush* brush() const;
    void setBrush(FwBrush* brush);

    virtual void apply(FwMLObject *object);


protected:
    void geometryChangedEvent(const QRect &oldRect, QRect &rect);

    void paint(FwPainter* painter, const QRect& clipRect);

    FwDrawer* createDrawer(const QByteArray& name, FwMLObject *object) const;

private:
    FwBrush* m_brush;
};

FwBrush* FwRectPrimitive::brush() const
{
    return m_brush;
}

#endif // FIREWORKS_GRAPHICSRECTITEM_H
