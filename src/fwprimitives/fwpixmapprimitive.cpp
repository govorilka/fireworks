#include <QtCore/qdebug.h>

#include "fwpixmapprimitive.h"

#include "fwgui/fwcanvas.h"

FwGraphicsPixmapItem::FwGraphicsPixmapItem(FwPrimitiveGroup* parent) :
    BaseClass(parent)
{
}

void FwGraphicsPixmapItem::setPixmap(const FwPixmap& pixmap)
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

QRect FwGraphicsPixmapItem::updateGeometry(const QRect &rect)
{
    return QRect(rect.topLeft(), m_pixmap.size());
}

void FwGraphicsPixmapItem::paint(FwCanvas *canvas)
{
    qDebug() << "FwGraphicsPixmapItem::paint" << boundingRect();
    QPoint pos = boundingRect().topLeft();
    canvas->drawPixmap(pos.x(), pos.y(), m_pixmap);
}
