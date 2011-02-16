#include <QtCore/qdebug.h>

#include "fwslidingframeprimitive.h"

#include "fwprimitives/fwstringprimitive.h"

#include "fwcore/fwml.h"

FwSlidingFramePrimitive::FwSlidingFramePrimitive(const QByteArray& name, FwPrimitiveGroup* parent) :
    BaseClass(name, parent),
    m_itemMargin(0),
    m_current(0),
    m_itemTemplate(0)
{
}

FwSlidingFramePrimitive::~FwSlidingFramePrimitive()
{
    delete m_itemTemplate;
}

void FwSlidingFramePrimitive::addItem(FwPrimitive* primitive, const QVariant& data)
{
    Q_UNUSED(data);
    if(!m_items.contains(primitive))
    {
        prepareGeometryChanged();

        primitive->link(geometry());
        if(m_itemTemplate)
        {
            primitive->apply(m_itemTemplate);
        }
        m_items.append(primitive);

        if(!m_current)
        {
            m_current = primitive;
        }

        update();
    }
}

FwStringPrimitive* FwSlidingFramePrimitive::addItem(const QString& text, const QVariant& data)
{
    if(!text.isEmpty())
    {
        FwStringPrimitive* string = new FwStringPrimitive("", this);
        string->setString(text);
        addItem(string, data);
        return string;
    }
    return 0;
}

void FwSlidingFramePrimitive::updateLayout(const QList<FwPrimitive*> items, FwPrimitive* current, const QRect& rect)
{
    int leftNullX = 0;
    int rightNullX = rect.width();

    int leftX = (rect.width() - current->width()) / 2;
    int rightX = leftX + current->width() + m_itemMargin;
    current->setPos(leftX, (rect.height() - current->height()) / 2);

    QList<FwPrimitive*>::const_iterator leftIter = items.begin();
    leftIter += items.indexOf(current);
    if(leftIter != items.begin())
    {
        do
        {
            --leftIter;
            FwPrimitive* primitive = (*leftIter);
            primitive->setPos(leftX -= (primitive->width() + m_itemMargin),
                              (rect.height() - primitive->height()) / 2);
        }
        while(leftIter != items.begin() && leftX >= leftNullX);
    }

    QList<FwPrimitive*>::const_iterator rightIter = items.begin();
    rightIter += (items.indexOf(current) + 1);
    while(rightIter != items.end() && rightX <= rightNullX)
    {
        FwPrimitive* primitive = (*rightIter);
        primitive->setPos(rightX,
                          (rect.height() - primitive->height()) / 2);
        rightX += (primitive->width() + m_itemMargin);
        rightIter++;
    }

    QList<FwPrimitive*>::const_iterator iter = items.begin();
    while(iter != leftIter)
    {
        FwPrimitive* primitive = (*iter);
        primitive->setPos(rightX,
                          (rect.height() - primitive->height()) / 2);
        rightX += (primitive->width() + m_itemMargin);
        iter++;
    }

    if(rightIter != items.end())
    {
        QList<FwPrimitive*>::const_iterator iter = items.end();
        do
        {
            --iter;
            FwPrimitive* primitive = (*iter);
            primitive->setPos(leftX -= (primitive->width() + m_itemMargin),
                              (rect.height() - primitive->height()) / 2);
        }
        while(iter != rightIter);
    }
}

void FwSlidingFramePrimitive::setCurrent(FwPrimitive* primitive)
{
    if(m_current != primitive)
    {
        prepareGeometryChanged();
        m_current = primitive;
        updateChildRect();
        update();
    }
}

void FwSlidingFramePrimitive::geometryChanged(const QRect &oldRect, QRect &rect)
{
    BaseClass::geometryChanged(oldRect, rect);
    if(m_current)
    {
        updateLayout(m_items, m_current, rect);
    }
}

void FwSlidingFramePrimitive::setItemMargin(int margin)
{
    if(m_itemMargin != margin)
    {
        prepareGeometryChanged();
        m_itemMargin = margin;
        update();
    }
}

void FwSlidingFramePrimitive::apply(FwMLObject *object)
{
    prepareGeometryChanged();

    FwMLNode* marginNode = object->attribute("itemMargin");
    if(marginNode)
    {
        bool bOk = false;
        int margin = marginNode->toInt(&bOk);
        if(bOk)
        {
            setItemMargin(margin);
        }
    }

    FwMLObject* itemNode = object->attribute("item")->cast<FwMLObject>();
    if(itemNode)
    {
        setItemTemplate(itemNode->clone()->cast<FwMLObject>());
    }

    BaseClass::apply(object);

    update();
}

void FwSlidingFramePrimitive::setItemTemplate(FwMLObject* itemTemplate)
{
    if(m_itemTemplate != itemTemplate)
    {
        delete m_itemTemplate;
        m_itemTemplate = itemTemplate;
        foreach(FwPrimitive* item, m_items)
        {
            item->apply(m_itemTemplate);
        }
    }
}

void FwSlidingFramePrimitive::invalidateChildrenRect()
{
    if(m_current)
    {
        updateLayout(m_items, m_current, geometryRect());
    }
    BaseClass::invalidateChildrenRect();
}
