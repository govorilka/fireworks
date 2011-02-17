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
        m_view->updateChildRect();
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
        m_view->updateChildRect();
        m_view->update();
    }
}

void FwSlidingFrameLayout::update(const QList<FwPrimitive*> items, FwPrimitive* current, const QRect& rect)
{
    int leftNullX = 0;
    int rightNullX = rect.width();

    int leftX = (rect.width() - current->width()) / 2;
    int rightX = leftX + current->width() + m_margin;
    current->setPos(leftX, (rect.height() - current->height()) / 2);

    QList<FwPrimitive*>::const_iterator leftIter = items.begin();
    leftIter += items.indexOf(current);
    if(leftIter != items.begin())
    {
        do
        {
            --leftIter;
            FwPrimitive* primitive = (*leftIter);
            primitive->setPos(leftX -= (primitive->width() + m_margin),
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
        rightX += (primitive->width() + m_margin);
        rightIter++;
    }

    QList<FwPrimitive*>::const_iterator iter = items.begin();
    while(iter != leftIter)
    {
        FwPrimitive* primitive = (*iter);
        primitive->setPos(rightX,
                          (rect.height() - primitive->height()) / 2);
        rightX += (primitive->width() + m_margin);
        iter++;
    }

    if(rightIter != items.end())
    {
        QList<FwPrimitive*>::const_iterator iter = items.end();
        do
        {
            --iter;
            FwPrimitive* primitive = (*iter);
            primitive->setPos(leftX -= (primitive->width() + m_margin),
                              (rect.height() - primitive->height()) / 2);
        }
        while(iter != rightIter);
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
