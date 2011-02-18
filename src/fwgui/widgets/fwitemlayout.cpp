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
    m_margin(0),
    m_criticalPoint(0),
    m_middleItem(0)
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

void FwSlidingFrameLayout::init(const QList<FwPrimitive*> items, FwPrimitive* current, const QRect& rect)
{
    m_criticalPoint = rect.width();
    foreach(FwPrimitive* primitive, items)
    {
        primitive->setY(0.5 * (rect.height() - primitive->height()));
        m_criticalPoint += primitive->width();
    }
    m_criticalPoint = 0.5 * (m_criticalPoint + m_margin * (items.size() - 1));

    m_middleItem = current;
}

void FwSlidingFrameLayout::update(const QList<FwPrimitive*> items, FwPrimitive* current, const QRect& rect)
{
    int leftX = (rect.width() - current->width()) / 2;
    int rightX = leftX + current->width() + m_margin;
    current->setX(leftX);

    if(items.size() == 1)
    {
        return;
    }

    QList<FwPrimitive*>::const_iterator currentIter = items.begin();
    currentIter += items.indexOf(current);

    QList<FwPrimitive*>::const_iterator beginIter = items.begin();
    QList<FwPrimitive*>::const_iterator rightIter = currentIter + 1;

    while(rightIter != items.end() && rightX <= m_criticalPoint)
    {
        FwPrimitive* primitive = (*rightIter);
        primitive->setX(rightX);
        rightX += (primitive->width() + m_margin);
        ++rightIter;
    }
    if(rightIter == items.end())
    {
        while(beginIter != currentIter && rightX <= m_criticalPoint)
        {
            FwPrimitive* primitive = (*beginIter);
            primitive->setX(rightX);
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
            primitive->setX(leftX -= (primitive->width() + m_margin));
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
            primitive->setX(leftX -= (primitive->width() + m_margin));
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
