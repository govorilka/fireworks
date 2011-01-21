#ifndef FIREWORKS_GRAPHICSPIXMAPITEM_H
#define FIREWORKS_GRAPHICSPIXMAPITEM_H

#include "fwprimitives/fwprimitive.h"

#include "fwgui/fwpixmap.h"

class FwGraphicsPixmapItem : public FwPrimitive
{
    typedef FwPrimitive BaseClass;

public:
    FwGraphicsPixmapItem(FwPrimitiveGroup* parent);

    inline FwPixmap pixmap() const;
    void setPixmap(const FwPixmap& pixmap);

protected:
    QRect updateGeometry(const QRect &rect);

    void paint(FwCanvas *canvas);

private:
    FwPixmap m_pixmap;
};

FwPixmap FwGraphicsPixmapItem::pixmap() const
{
    return m_pixmap;
}

#endif //FIREWORKS_GRAPHICSPIXMAPITEM_H
