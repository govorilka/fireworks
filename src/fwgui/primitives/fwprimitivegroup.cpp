#include <QtCore/qdebug.h>

#include "fwprimitivegroup.h"
#include "fwdrawer.h"

#include "fwcore/fwml.h"

#include "fwgui/fwpainter.h"
#include "fwgui/fwgraphicsobject.h"
#include "fwgui/fwscene.h"
#include "fwgui/fwgraphicsview.h"

FwPrimitiveGroup::FwPrimitiveGroup(const QByteArray& name, FwPrimitiveGroup* parent) :
    BaseClass(name, parent),
    m_childrenRect(0, 0, 0, 0),
    childrenDirty(false),
    m_invalidateChildrenRect(false)
{
    if(parent)
    {
        parent->m_groups.append(this);
    }
}

FwPrimitiveGroup::~FwPrimitiveGroup()
{
    removeItems();
    if(m_parent)
    {
        m_parent->m_groups.removeOne(this);
    }
}

void FwPrimitiveGroup::removeItems()
{
    if(!m_primitives.isEmpty())
    {
        m_visiblePrimitives.clear();

        m_scene->m_view->m_dirtyRegion->pushObjectRect(object()->geometry()->rect());

        foreach(FwPrimitive* item, m_primitives)
        {
            Q_ASSERT(item->m_parent == this);
            m_scene->m_view->m_dirtyRegion->addChildrenRect(item->m_boundingRect);
            item->m_parent = 0;
            delete item;
        }
        m_primitives.clear();

        m_scene->m_view->m_dirtyRegion->popObjectRect();
        m_scene->m_view->postUpdateEvent();

        m_childrenRect = QRect();
    }
}

void FwPrimitiveGroup::paint(FwPainter *painter, const QRect &clipRect)
{
    BaseClass::paint(painter, clipRect);

    QRect newClipRect = clipRect.intersect(m_childrenRect);
    if(!newClipRect.isEmpty())
    {
        QRect oldClipRect = painter->setClipRect(newClipRect);
        foreach(FwPrimitive* item, m_visiblePrimitives)
        {
            QRect childClipRect = newClipRect.intersect(item->m_boundingRect);
            if(!childClipRect.isEmpty())
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
                                        childClipRect.translated(item->m_boundingRect.topLeft()));
                }
                else
                {
                    if(item->m_drawer)
                    {
                        item->m_drawer->paint(painter, childClipRect);
                    }
                    item->paint(painter, childClipRect);
                }
            }
        }
        painter->restoreClipRect(oldClipRect);
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

    m_invalidateChildrenRect = true;
    updateChildren();
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
    return m_parent ? m_parent->object() : m_scene;
}

void FwPrimitiveGroup::invalidateChildren()
{
    if(childrenDirty)
    {
        m_scene->m_view->m_dirtyRegion->pushObjectRect(object()->geometry()->rect());

        while(childrenDirty)
        {
            childrenDirty = false;

            if(needSortZIndex)
            {
                qSort(m_primitives.begin(), m_primitives.end(), FwPrimitive::zIndexLessThan);
                needSortZIndex = false;
            }

            foreach(FwPrimitiveGroup* group, m_groups)
            {
                group->invalidateChildren();
            }

            int childrenPosChanged = 0;
            int childrenSizeChanged = 0;

            foreach(FwPrimitive* primitive, m_primitives)
            {
                if(primitive->m_contentDirty)
                {
                    if(primitive->m_geometry->isDirty())
                    {
                        childrenPosChanged += primitive->m_geometry->posChanged();
                        childrenSizeChanged += primitive->m_geometry->sizeChanged();
                        primitive->invalidateGeometry();
                    }
                    m_scene->m_view->m_dirtyRegion->addChildrenRect(primitive->m_boundingRect);
                    primitive->m_contentDirty = false;
                }
            }

            if(childrenPosChanged || childrenSizeChanged)
            {
                childrenRectChangedEvent(childrenPosChanged, childrenSizeChanged);
                m_invalidateChildrenRect = true;
            }

            if(m_invalidateChildrenRect)
            {
                invalidateChildrenRect();
                m_invalidateChildrenRect = false;
            }
        }

        m_scene->m_view->m_dirtyRegion->popObjectRect();
    }
}

void FwPrimitiveGroup::invalidateChildrenRect()
{
    m_visiblePrimitives.clear();

    if(object() != this)
    {
        int x1 = 0;
        int y1 = 0;
        int x2 = 0;
        int y2 = 0;
        foreach(FwPrimitive* primitive, m_primitives)
        {
            if(primitive->visibleOnScreen)
            {
                m_visiblePrimitives.append(primitive);
                x1 = qMin(x1, primitive->m_geometry->rect().left());
                y1 = qMin(y1, primitive->m_geometry->rect().top());
                x2 = qMax(x2, primitive->m_geometry->rect().right());
                y2 = qMax(y2, primitive->m_geometry->rect().bottom());
            }
        }
        foreach(FwPrimitiveGroup* group, m_groups)
        {
            if(group->visibleOnScreen)
            {
                x1 = qMin(x1, group->m_childrenRect.left());
                y1 = qMin(y1, group->m_childrenRect.top());
                x2 = qMax(x2, group->m_childrenRect.right());
                y2 = qMax(y2, group->m_childrenRect.bottom());
            }
        }
        m_childrenRect.setCoords(x1, y1, x2, y2);
    }
    else
    {
        foreach(FwPrimitive* primitive, m_primitives)
        {
            QRect rect = m_childrenRect.intersected(primitive->m_boundingRect);
            if(primitive->visibleOnScreen && !rect.isNull())
            {
                m_visiblePrimitives.append(primitive);
            }
        }
    }
}

FwPrimitive* FwPrimitiveGroup::primitiveByName(const QList<QByteArray>& name, int firstElement)
{
    foreach(FwPrimitive* primitive, m_primitives)
    {
        if(primitive->name() == name[firstElement])
        {
            if(firstElement < (name.size() - 1))
            {
                return primitive->primitiveByName(name, firstElement + 1);
            }
            else
            {
                return primitive;
            }
        }
    }
    return 0;
}

void FwPrimitiveGroup::childrenRectChangedEvent(bool posChanged, bool sizeChanged)
{
    Q_UNUSED(posChanged);
    Q_UNUSED(sizeChanged);
}
