#include <QtCore/qdebug.h>

#include "fwslidingframeprimitive.h"

FwSlidingFramePrimitive::FwSlidingFramePrimitive(const QByteArray& name, FwPrimitiveGroup* parent) :
    BaseClass(name, parent),
    m_itemMargin(0),
    m_current(0)
{
}

FwSlidingFramePrimitive::~FwSlidingFramePrimitive()
{
}

void FwSlidingFramePrimitive::addItem(FwPrimitive* primitive)
{
    if(!m_items.contains(primitive))
    {
        prepareGeometryChanged();

        primitive->link(geometry());
        m_items.append(primitive);

        if(!m_current)
        {
            m_current = primitive;
        }

        update();
    }
}

void FwSlidingFramePrimitive::updateLayout(const QList<FwPrimitive*> items, FwPrimitive* current)
{
    int currentX = (width() - current->width()) / 2;
    current->setPos(currentX, (height() - current->height()) / 2);

    QList<FwPrimitive*>::const_iterator currentIter = items.begin();
    currentIter += items.indexOf(current);

    if(currentIter != items.begin())
    {
        QList<FwPrimitive*>::const_iterator iter = currentIter;
        do
        {
            --iter;
            FwPrimitive* primitive = (*iter);
            primitive->setPos(currentX -= (primitive->width() - m_itemMargin),
                              (height() - primitive->height()) / 2);
        }
        while(iter != items.begin());
    }

    currentX = current->rect().right() + m_itemMargin;

    QList<FwPrimitive*>::const_iterator iter = ++currentIter;
    while(iter != items.end())
    {
        FwPrimitive* primitive = (*iter);
        primitive->setPos(currentX,
                          (height() - primitive->height()) / 2);
        currentX += (primitive->width() + m_itemMargin);
        iter++;
    }
}

void FwSlidingFramePrimitive::setCurrent(FwPrimitive* primitive)
{
    if(m_items.contains(primitive) && m_current != primitive)
    {
        prepareGeometryChanged();
        m_current = primitive;
        updateLayout(m_items, m_current);
        update();
    }
}

QRect FwSlidingFramePrimitive::updateGeometry(const QRect &rect)
{
    if(m_current)
    {
        updateLayout(m_items, m_current);
    }
    return BaseClass::updateGeometry(rect);
}
