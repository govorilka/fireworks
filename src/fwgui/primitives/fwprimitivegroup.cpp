#include <QtCore/qdebug.h>

#include <QtGui/qregion.h>

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
    childrenRectDirty(false),
    childSizeChanged(false)
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
    m_visiblePrimitives.clear();

    foreach(FwPrimitive* item, m_primitives)
    {
        Q_ASSERT(item->m_parent == this);
        item->m_parent = 0;
        delete item;
    }
    m_primitives.clear();
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
            QRect childClipRect = newClipRect.intersect(item->geometry()->rect());
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
                                        childClipRect.translated(item->geometry()->rect().topLeft()));
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
    qDebug() << "FwPrimitiveGroup::visibleChangedEvent" << visibleOnScreen;
    foreach(FwPrimitive* item, m_primitives)
    {
        if(item->visibleOnScreen != (visibleOnScreen && item->m_visible))
        {
            item->visibleOnScreen = !item->visibleOnScreen;
            item->visibleChangedEvent();
        }
    }
    updateChildrenRect();
}

void FwPrimitiveGroup::apply(FwMLObject *object)
{
    prepareGeometryChanged();

    foreach(FwPrimitive* primitive, m_primitives)
    {
        if(!primitive->name().isEmpty())
        {
            qDebug() << primitive->name();
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

void FwPrimitiveGroup::invalidateChildren()
{
    if(childrenDirty)
    {
        foreach(FwPrimitiveGroup* group, m_groups)
        {
            group->invalidateChildren();
        }

        if(needSortZIndex)
        {
            if(m_primitives.size() > 1)
            {
                qSort(m_primitives.begin(), m_primitives.end(), FwPrimitive::zIndexLessThan);
            }
            needSortZIndex = false;
        }

        if(childrenRectDirty)
        {
            invalidateChildrenRect();
            childSizeChanged = false;
            childrenRectDirty = false;
        }

        childrenDirty = false;
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
                primitive->invalidateCanvas(m_childrenRect);

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
                primitive->invalidateCanvas(m_childrenRect);
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
