#ifndef FIREWORKS_GRAPHICSPIXMAPITEM_H
#define FIREWORKS_GRAPHICSPIXMAPITEM_H

#include "fwprimitives/fwprimitive.h"

#include "fwgui/fwpixmap.h"

class FwPixmapPrimitive : public FwPrimitive
{
    typedef FwPrimitive BaseClass;

public:
    FwPixmapPrimitive(const QByteArray& name, FwPrimitiveGroup* parent);

    inline FwPixmap pixmap() const;
    void setPixmap(const FwPixmap& pixmap);

    void apply(FwMLObject *object);

protected:
    QRect updateGeometry(const QRect &rect);

    void paint(FwPainter *painter, const QRect &clipRect);

private:
    FwPixmap m_pixmap;
};

FwPixmap FwPixmapPrimitive::pixmap() const
{
    return m_pixmap;
}

#endif //FIREWORKS_GRAPHICSPIXMAPITEM_H
