#include <QtCore/qdebug.h>

#include <QtGui/qregion.h>

#include "fwprimitivegroup.h"

#include "fwcore/fwml.h"

#include "fwgui/fwpainter.h"
#include "fwgui/fwgraphicsobject.h"

FwPrimitiveGroup::FwPrimitiveGroup(const QByteArray& name, FwPrimitiveGroup* parent) :
    BaseClass(name, parent),
    m_childrenRect(0, 0, 0, 0),
    m_childrenRectDirty(false)
{
    if(parent)
    {
        parent->m_groups.append(this);
    }
}

FwPrimitiveGroup::~FwPrimitiveGroup()
{
    if(m_parent)
    {
        m_parent->m_groups.removeOne(this);
    }
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

void FwPrimitiveGroup::paint(FwPainter *painter, const QRect &clipRect)
{
    BaseClass::paint(painter, clipRect);
    if(clipRect.intersects(m_childrenRect))
    {
        foreach(FwPrimitive* item, m_primitives)
        {
            if(item->visibleOnScreen)
            {
                QRect newClipRect = clipRect.intersected(item->geometryRect());
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
                                            newClipRect.translated(item->geometryRect().topLeft()));
                    }
                    else
                    {
                        item->paint(painter, newClipRect);
                    }
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

static inline void uniteRect(QRect& rect, const QRect& addRect)
{
    rect.setCoords(qMin(rect.left(), addRect.left()),
                   qMin(rect.top(), addRect.top()),
                   qMax(rect.right(), addRect.right()),
                   qMax(rect.bottom(), addRect.bottom()));
}

void FwPrimitiveGroup::invalidateChildrenRect()
{
    if(m_childrenRectDirty)
    {
        foreach(FwPrimitiveGroup* group, m_groups)
        {
            group->invalidateChildrenRect();
        }

        if(object() != this)
        {
            int x1 = 0;
            int y1 = 0;
            int x2 = 0;
            int y2 = 0;
            foreach(FwPrimitive* primitive, m_primitives)
            {
                x1 = qMin(x1, primitive->m_geometry->rect().left());
                y1 = qMin(y1, primitive->m_geometry->rect().top());
                x2 = qMax(x2, primitive->m_geometry->rect().right());
                y2 = qMax(y2, primitive->m_geometry->rect().bottom());
            }
            foreach(FwPrimitiveGroup* group, m_groups)
            {
                x1 = qMin(x1, group->m_childrenRect.left());
                y1 = qMin(y1, group->m_childrenRect.top());
                x2 = qMax(x2, group->m_childrenRect.right());
                y2 = qMax(y2, group->m_childrenRect.bottom());
            }
            m_childrenRect.setCoords(x1, y1, x2, y2);
        }
        m_childrenRectDirty = false;
    }
}