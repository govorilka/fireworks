#ifndef FIREWORKS_GRAPHICSPIXMAPITEM_H
#define FIREWORKS_GRAPHICSPIXMAPITEM_H

#include "fwgui/primitives/fwprimitive.h"
#include "fwgui/fwpixmap.h"

class FIREWORKSSHARED_EXPORT FwPixmapPrimitive : public FwPrimitive
{
    typedef FwPrimitive BaseClass;

public:
    FwPixmapPrimitive(const QByteArray& name, FwPrimitiveGroup* parent);

    inline FwPixmap pixmap() const;
    void setPixmap(const FwPixmap& pixmap);
    void setPixmap(const QByteArray& name);

    void apply(FwMLObject *object);

protected:
    void boundingRectChangedEvent(QRect &boundingRect);

    void paint(FwPainter *painter, const QRect &clipRect);

private:
    FwPixmap m_pixmap;
};

FwPixmap FwPixmapPrimitive::pixmap() const
{
    return m_pixmap;
}

#endif //FIREWORKS_GRAPHICSPIXMAPITEM_H
