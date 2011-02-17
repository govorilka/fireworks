#include <QtCore/qdebug.h>

#include "fwitemlayout.h"
#include "fwitemview.h"

#include"fwcore/fwml.h"

FwItemLayout::FwItemLayout(FwItemView* view) :
    BaseClass(view),
    m_view(view)
{
}

///////////////////////////////////////////////////////////////////////

const char FwSlidingFrameLayout::staticClassName[] = "fireworks.layouts.SlidingFrame";

FwSlidingFrameLayout::FwSlidingFrameLayout(FwItemView* view) :
    BaseClass(view),
    m_orientation(Fw::O_Horizontal),
    m_margin(0)
{
}

QByteArray FwSlidingFrameLayout::className() const
{
    return staticClassName;
}

FwItemLayout* FwSlidingFrameLayout::constructor(FwItemView* view)
{
    return new FwSlidingFrameLayout(view);
}

bool FwSlidingFrameLayout::setOrientation(Fw::Orientation orientation)
{
    if(orientation != Fw::O_Horizontal || orientation != Fw::O_Vertical)
    {
        return false;
    }

    if(m_orientation != orientation)
    {
        m_view->prepareGeometryChanged();
        m_orientation = orientation;
        m_view->updateChildrenRect();
        m_view->update();
    }
    return true;
}

void FwSlidingFrameLayout::setMargin(int margin)
{
    if(m_margin != margin)
    {
        m_view->prepareGeometryChanged();
        m_margin = margin;
        m_view->updateChildrenRect();
        m_view->update();
    }
}

void FwSlidingFrameLayout::update(const QList<FwPrimitive*> items, FwPrimitive* current, const QRect& rect)
{
    int maxWidth = rect.width();
    foreach(FwPrimitive* primitive, items)
    {
        maxWidth += primitive->width();
    }
    maxWidth = 0.5 * (maxWidth + m_margin * (items.size() - 1));

    int leftNullX = 0;
    int rightNullX = maxWidth;

    int leftX = (rect.width() - current->width()) / 2;
    int rightX = leftX + current->width() + m_margin;
    current->setPos(leftX, (rect.height() - current->height()) / 2);

    if(items.size() == 1)
    {
        return;
    }

    QList<FwPrimitive*>::const_iterator currentIter = items.begin();
    currentIter += items.indexOf(current);

    QList<FwPrimitive*>::const_iterator beginIter = items.begin();
    QList<FwPrimitive*>::const_iterator rightIter = currentIter + 1;

    while(rightIter != items.end() && rightX <= rightNullX)
    {
        FwPrimitive* primitive = (*rightIter);
        primitive->setPos(rightX,
                          (rect.height() - primitive->height()) / 2);
        rightX += (primitive->width() + m_margin);
        ++rightIter;
    }
    if(rightIter == items.end())
    {
        while(beginIter != currentIter && rightX <= rightNullX)
        {
            FwPrimitive* primitive = (*beginIter);
            primitive->setPos(rightX,
                              (rect.height() - primitive->height()) / 2);
            rightX += (primitive->width() + m_margin);
            ++beginIter;
        }
    }
    if(beginIter != currentIter)
    {
        QList<FwPrimitive*>::const_iterator leftIter = currentIter;
        do
        {
            --leftIter;
            FwPrimitive* primitive = (*leftIter);
            primitive->setPos(leftX -= (primitive->width() + m_margin),
                              (rect.height() - primitive->height()) / 2);
        }
        while(leftIter != beginIter);
    }
    if(rightIter != items.end())
    {
        QList<FwPrimitive*>::const_iterator leftIter = items.end();
        do
        {
            --leftIter;
            FwPrimitive* primitive = (*leftIter);
            primitive->setPos(leftX -= (primitive->width() + m_margin),
                              (rect.height() - primitive->height()) / 2);
        }
        while(leftIter != rightIter);
    }
}

void FwSlidingFrameLayout::apply(FwMLObject* object)
{
    FwMLNode* marginNode = object->attribute("margin");
    if(marginNode)
    {
        bool bOk = false;
        int margin = marginNode->toInt(&bOk);
        if(bOk)
        {
            setMargin(margin);
        }
    }
}
