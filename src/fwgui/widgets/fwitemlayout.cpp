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
    m_startPoint(0),
    m_endPoint(0),
    m_middleItem(0),

    m_loopTimer(0),
    m_startX(0),
    m_endX(0),
    m_step(0),
    m_delta(0)
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

void FwSlidingFrameLayout::init(const QList<FwPrimitive*> items, const QRect& rect)
{
    m_criticalPoint = rect.width();
    foreach(FwPrimitive* primitive, items)
    {
        primitive->setY(0.5 * (rect.height() - primitive->height()));
        m_criticalPoint += primitive->width();
    }
    m_criticalPoint = 0.5 * (m_criticalPoint + m_margin * (items.size() - 1));
    m_middleItem = 0;
}

void FwSlidingFrameLayout::update(const QList<FwPrimitive*> items, FwPrimitive* current, const QRect& rect)
{
    if(!m_middleItem)
    {
        m_middleItem = current;
        m_middleItem->setX((rect.width() - current->width()) / 2);
        if(items.size() != 1)
        {
            calculateItemPosition(items);
        }
    }
    else
    {
        QList<FwPrimitive*>::const_iterator iter = items.begin();
        iter += items.indexOf(m_middleItem);

        QList<FwPrimitive*>::const_iterator currentIter = items.begin();
        currentIter += items.indexOf(current);

        int startX = m_middleItem->x();
        if(m_middleItem->x() < current->x())
        {
            do
            {
                ++iter;
                if(iter == items.end())
                {
                    iter = items.begin();
                }
                startX += ((*iter)->width() + m_margin);
            }
            while(iter != currentIter);
        }
        else
        {
            do
            {
                if(iter == items.begin())
                {
                    iter = items.end();
                }
                --iter;
                startX -= ((*iter)->width() + m_margin);
            }
            while(iter != currentIter);
        }
        loop(startX, (rect.width() - current->width()) * 0.5);
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

void FwSlidingFrameLayout::loop(int startX, int endX)
{
    m_startX = startX;
    m_endX = endX;
    m_step = startX > endX ? -1 : 1;
    m_delta = m_startX;

    if(m_loopTimer)
    {
        killTimer(m_loopTimer);
    }
    m_loopTimer = startTimer(10);
}

void FwSlidingFrameLayout::timerEvent(QTimerEvent *e)
{
    Q_UNUSED(e);

    m_delta += m_step;
    if(m_delta == m_endX)
    {
        killTimer(m_loopTimer);
        m_middleItem = currentItem();
        m_loopTimer = 0;
        m_startX = 0;
        m_endX = 0;
        m_step = 0;
        m_delta = 0;
    }
    move(m_step);
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

void FwSlidingFrameLayout::move(int delta)
{
    m_startPoint += delta;
    m_endPoint += delta;
    if(m_startPoint > 0 || m_endPoint < rect().width())
    {
        m_middleItem = calculateMiddleItem(items(), rect());
        calculateItemPosition(items());
    }
    else
    {
        foreach(FwPrimitive* primitive, items())
        {
            primitive->setX(primitive->x() + delta);
        }
    }
}
