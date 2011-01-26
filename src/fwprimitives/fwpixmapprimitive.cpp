#include <QtCore/qdebug.h>

#include "fwpixmapprimitive.h"

#include "fwcore/fwml.h"

#include "fwgui/fwpainter.h"

FwPixmapPrimitive::FwPixmapPrimitive(FwPrimitiveGroup* parent) :
    BaseClass(parent)
{
}

void FwPixmapPrimitive::setPixmap(const FwPixmap& pixmap)
{
    if(m_pixmap != pixmap)
    {
         if(m_pixmap.size() == pixmap.size())
        {
            m_pixmap = pixmap;
            invalidate();
        }
        else
        {
            prepareGeometryChanged();
            m_pixmap = pixmap;
            setSize(m_pixmap.size());
            update();
        }
    }
}

QRect FwPixmapPrimitive::updateGeometry(const QRect &rect)
{
    return QRect(rect.topLeft(), m_pixmap.size());
}

void FwPixmapPrimitive::paint(FwPainter *painter, const QRect &clipRect)
{
    painter->drawPixmap(clipRect, m_pixmap, &clipRect.translated(boundingRect().topLeft()));
}

void FwPixmapPrimitive::apply(FwMLObject *object)
{
    prepareGeometryChanged();

    FwPixmap pixmap = createPixmap(object);
    if(!pixmap.isNull())
    {
        setPixmap(pixmap);
    }

    BaseClass::apply(object);

    update();
}
