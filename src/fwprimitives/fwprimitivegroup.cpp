#include <QtCore/qdebug.h>

#include <QtGui/qregion.h>

#include "fwprimitivegroup.h"

#include "fwgui/fwpainter.h"

FwPrimitiveGroup::FwPrimitiveGroup(FwPrimitiveGroup* parent) :
    BaseClass(parent)
{
}

FwPrimitiveGroup::~FwPrimitiveGroup()
{
    removeItems();
}

void FwPrimitiveGroup::removeItems()
{
    foreach(FwPrimitive* item, m_items)
    {
        Q_ASSERT(item->m_parent == this);
        item->m_parent = 0;
        delete item;
    }
    m_items.clear();

    if(scene())
    {
        invalidate();
    }
}

QRect FwPrimitiveGroup::updateGeometry(const QRect& rect)
{
    QRegion region;

    QRect baseRect = BaseClass::updateGeometry(rect);
    if(!baseRect.isNull())
    {
        region = region.united(baseRect);
    }

    foreach(FwPrimitive* item , m_items)
    {
        if(item->m_boundingRectDirty)
        {
            item->m_boundingRect = item->updateGeometry(item->rect());
            item->m_boundingRectDirty = false;
        }
        region = region.united(item->m_boundingRect);
    }

    return region.boundingRect();
}

void FwPrimitiveGroup::paint(FwPainter *painter, const QRect &clipRect)
{
    BaseClass::paint(painter, clipRect);

    foreach(FwPrimitive* item, m_items)
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
    foreach(FwPrimitive* item, m_items)
    {
        if(item->visibleOnScreen != (visibleOnScreen && item->m_visible))
        {
            item->visibleOnScreen = !item->visibleOnScreen;
            item->visibleChangedEvent();
        }
    }
}
