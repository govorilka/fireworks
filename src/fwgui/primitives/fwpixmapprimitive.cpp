#include <QtCore/qdebug.h>

#include "fwpixmapprimitive.h"

#include "fwcore/fwml.h"

#include "fwgui/fwpainter.h"
#include "fwgui/fwscene.h"
#include "fwgui/fwgraphicsview.h"
#include "fwgui/fwimagelibrary.h"

FwPixmapPrimitive::FwPixmapPrimitive(const QByteArray& name, FwPrimitiveGroup* parent) :
    BaseClass(name, parent)
{
}

void FwPixmapPrimitive::setPixmap(const FwPixmap& pixmap)
{
    if(m_pixmap != pixmap)
    {
        prepareGeometryChanged();
        m_pixmap = pixmap;
        setSize(m_pixmap.size());
        update();
    }
}

void FwPixmapPrimitive::setPixmap(const QByteArray& name)
{
    setPixmap(scene()->view()->imageLibrary()->image(name));
}

void FwPixmapPrimitive::boundingRectChangedEvent(QRect &boundingRect)
{
    boundingRect.setSize(m_pixmap.size());
}

void FwPixmapPrimitive::paint(FwPainter *painter, const QRect &clipRect)
{
    painter->drawPixmap(clipRect, m_pixmap, &clipRect.translated(-geometry()->rect().topLeft()));
}

void FwPixmapPrimitive::apply(FwMLObject *object)
{
    prepareGeometryChanged();

    FwMLString* sourceNode = object->attribute("source")->cast<FwMLString>();
    if(sourceNode && !sourceNode->isEmpty())
    {
        setPixmap(sourceNode->value());
    }

    BaseClass::apply(object);
    update();
}
