#include <QtCore/qdebug.h>

#include <QtGui/qregion.h>

#include "fwprimitivegroup.h"

#include "fwcore/fwml.h"

#include "fwgui/fwpainter.h"
#include "fwgui/fwgraphicsobject.h"

FwPrimitiveGroup::FwPrimitiveGroup(const QByteArray& name, FwPrimitiveGroup* parent) :
    BaseClass(name, parent)
{
}

FwPrimitiveGroup::~FwPrimitiveGroup()
{
    removeItems();
}

void FwPrimitiveGroup::removeItems()
{
    foreach(FwPrimitive* item, m_primitives)
    {
        Q_ASSERT(item->m_parent == this);
        item->m_parent = 0;
        delete item;
    }
    m_primitives.clear();

    if(scene())
    {
        invalidate();
    }
}

void FwPrimitiveGroup::updateGeometry(const QRect &rect, QRect& boundingRect)
{
    QRegion region;

    BaseClass::updateGeometry(rect, boundingRect);
    if(!boundingRect.isNull())
    {
        region = region.united(boundingRect);
    }

    foreach(FwPrimitive* primitive, m_primitives)
    {
        primitive->invalidateBoundingRect();
        region = region.united(primitive->m_boundingRect);
    }

    boundingRect = region.boundingRect();
}

void FwPrimitiveGroup::paint(FwPainter *painter, const QRect &clipRect)
{
    BaseClass::paint(painter, clipRect);
    foreach(FwPrimitive* item, m_primitives)
    {
        if(item->visibleOnScreen)
        {
            QRect newClipRect = clipRect.intersected(item->m_boundingRect);
            if(!newClipRect.isNull())
            {
                if(item->m_bufferMode)
                {
                    if(item->bufferDirty)
                    {
                        if(!item->m_buffer)
                        {
                            item->createNewBuffer();
                        }
                        else
                        {
                            item->updateBuffer();
                        }
                    }
                    painter->drawBuffer(newClipRect,
                                        item->m_buffer,
                                        newClipRect.translated(item->m_boundingRect.topLeft()));
                }
                else
                {
                    item->paint(painter, newClipRect);
                }
            }
        }
    }
}

void FwPrimitiveGroup::visibleChangedEvent()
{
    foreach(FwPrimitive* item, m_primitives)
    {
        if(item->visibleOnScreen != (visibleOnScreen && item->m_visible))
        {
            item->visibleOnScreen = !item->visibleOnScreen;
            item->visibleChangedEvent();
        }
    }
}

void FwPrimitiveGroup::apply(FwMLObject *object)
{
    prepareGeometryChanged();

    foreach(FwPrimitive* primitive, m_primitives)
    {
        if(!primitive->name().isEmpty())
        {
            FwMLObject* childObject = object->attribute(primitive->name())->cast<FwMLObject>();
            if(childObject)
            {
                primitive->apply(childObject);
            }
        }
    }

    BaseClass::apply(object);

    update();
}

FwGraphicsObject* FwPrimitiveGroup::object() const
{
    return m_parent ? m_parent->object() : 0;
}
