#include <QtCore/qdebug.h>

#include <QtGui/qregion.h>

#include "fwprimitivegroup.h"

#include "fwgui/fwcanvas.h"

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
    if(brush())
    {
        region = QRegion(rect);
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

void FwPrimitiveGroup::paint(FwCanvas* canvas)
{
    BaseClass::paint(canvas);

    QRect canvasRect = canvas->rect();

    foreach(FwPrimitive* item, m_items)
    {
        if(item->visibleOnScreen && canvasRect.intersects(item->m_boundingRect))
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

                FwRender* render = canvas->render();
                render->drawPixmap(render->startX() + item->m_boundingRect.x(),
                                   render->startY() + item->m_boundingRect.y(),
                                   item->m_buffer);
            }
            else
            {
                item->paint(canvas);
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
