#include <QtCore/qdebug.h>

#include "fwitemlayout.h"
#include "fwitemview.h"

#include"fwcore/fwml.h"

FwItemLayout::FwItemLayout(FwItemView* view) :
    BaseClass(view),
    m_view(view)
{
}

QList<FwPrimitive*> FwItemLayout::items() const
{
    return m_view->items();
}

FwPrimitive* FwItemLayout::currentItem() const
{
    return m_view->current();
}

QRect FwItemLayout::rect() const
{
    return m_view->rect();
}

///////////////////////////////////////////////////////////////////////

const char FwSlidingFrameLayout::staticClassName[] = "fireworks.layouts.SlidingFrame";

FwSlidingFrameLayout::FwSlidingFrameLayout(FwItemView* view) :
    BaseClass(view),
    m_orientation(Fw::O_Horizontal),
    m_margin(0),
    m_criticalPoint(0),
    m_totalLenght(0),
    m_startPoint(0),
    m_endPoint(0),
    m_middleItem(0),
    m_candidateItem(0),
    m_candidatePoint(0),

    m_animation(new QPropertyAnimation(this, "move", this)),
    m_deltaValue(0)
{
    m_animation->setEasingCurve(QEasingCurve::OutBounce);
    m_animation->setDuration(500);
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
    m_totalLenght = m_margin * (items.size() - 1);
    foreach(FwPrimitive* primitive, items)
    {
        primitive->setY(0.5 * (rect.height() - primitive->height()));
        m_totalLenght += primitive->width();
    }
    m_criticalPoint = rect.width() + 0.5 * m_totalLenght;

    m_middleItem = current;
    m_middleItem->setX((rect.width() - current->width()) / 2);
    if(items.size() != 1)
    {
        calculateItemPosition(items);
    }

    m_candidateItem = current;
    m_candidatePoint = m_candidateItem->x();
}

void FwSlidingFrameLayout::update(const QList<FwPrimitive*> items, FwPrimitive* current, const QRect& rect)
{
    int endX = (rect.width() - current->width()) * 0.5;
    m_deltaValue = current->x();

    m_candidateItem = current;

    m_animation->setStartValue(m_deltaValue);
    if(m_animation->state() != QAbstractAnimation::Running)
    {
        m_animation->setDuration(500);
        m_animation->setEndValue(endX);
        m_animation->start();
    }
    else
    {
        m_animation->setDuration(m_animation->currentTime() + 500);
    }
}

void FwSlidingFrameLayout::calculateItemPosition(const QList<FwPrimitive*>& items)
{
    m_startPoint = m_middleItem->x();
    m_endPoint = m_startPoint + m_middleItem->width() + m_margin;

    QList<FwPrimitive*>::const_iterator currentIter = items.begin();
    currentIter += items.indexOf(m_middleItem);

    QList<FwPrimitive*>::const_iterator beginIter = items.begin();
    QList<FwPrimitive*>::const_iterator rightIter = currentIter + 1;

    while(rightIter != items.end() && m_endPoint <= m_criticalPoint)
    {
        FwPrimitive* primitive = (*rightIter);
        primitive->setX(m_endPoint);
        m_endPoint += (primitive->width() + m_margin);
        ++rightIter;
    }
    if(rightIter == items.end())
    {
        while(beginIter != currentIter && m_endPoint <= m_criticalPoint)
        {
            FwPrimitive* primitive = (*beginIter);
            primitive->setX(m_endPoint);
            m_endPoint += (primitive->width() + m_margin);
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
            primitive->setX(m_startPoint -= (primitive->width() + m_margin));
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
            primitive->setX(m_startPoint -= (primitive->width() + m_margin));
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

void FwSlidingFrameLayout::loop()
{
}

FwPrimitive* FwSlidingFrameLayout::calculateMiddleItem(const QList<FwPrimitive*>& items, const QRect& rect)
{
    FwPrimitive* middleItem = 0;
    int halfWidth = rect.width() / 2;
    int delta = halfWidth;
    foreach(FwPrimitive* primitive, items)
    {
        int newDelta = qMin(qAbs(halfWidth - primitive->geometryRect().right()),
                            qAbs(primitive->geometryRect().left() - halfWidth));
        if(newDelta < delta)
        {
            delta = newDelta;
            middleItem = primitive;
        }
    }
    return middleItem;
}

void FwSlidingFrameLayout::move(int point)
{
    int step = point - m_deltaValue;
    m_deltaValue = point;

    m_startPoint += step;
    m_endPoint += step;
    if(m_startPoint > 0 || m_endPoint < rect().width())
    {
        m_middleItem = calculateMiddleItem(items(), rect());
        calculateItemPosition(items());
    }
    else
    {
        foreach(FwPrimitive* primitive, items())
        {
            primitive->setX(primitive->x() + step);
        }
    }
}
