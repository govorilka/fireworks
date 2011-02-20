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
    m_candidateItem(0),
    m_nextCandidateItem(0),

    m_animation(new QPropertyAnimation(this, "move", this)),
    m_deltaValue(0)
{
    m_animation->setDuration(500);
    connect(m_animation, SIGNAL(finished()), this, SLOT(animationFinish()));
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
    m_criticalPoint = rect.width() + m_margin * (items.size() - 1);
    foreach(FwPrimitive* primitive, items)
    {
        primitive->setY(0.5 * (rect.height() - primitive->height()));
        m_criticalPoint += primitive->width();
    }
    m_criticalPoint = 0.5 * m_criticalPoint;

    m_candidateItem = current;
    m_candidateItem->setX(m_candidatePoint = ((rect.width() - current->width()) * 0.5));
    if(items.size() != 1)
    {
        calculateItemPosition(items, m_candidateItem);
    }
}

void FwSlidingFrameLayout::nextItem(const QList<FwPrimitive*>& items, int key)
{
    QList<FwPrimitive*>::const_iterator candidateIter = items.begin();

    switch(key)
    {
    case Qt::Key_Right:
        candidateIter += (items.indexOf(m_candidateItem) + 1);
        if(candidateIter == items.end())
        {
            candidateIter = items.begin();
        }
        break;

    case Qt::Key_Left:
        if(m_candidateItem == (*candidateIter))
        {
            candidateIter = --items.end();
        }
        else
        {
            candidateIter += (items.indexOf(m_candidateItem) - 1);
        }
        break;

    default:
        return;
    }

    animationStart(*candidateIter);
}

void FwSlidingFrameLayout::calculateItemPosition(const QList<FwPrimitive*>& items, FwPrimitive* middleItem)
{
    m_startPoint = middleItem->x();
    m_endPoint = m_startPoint + middleItem->width() + m_margin;

    QList<FwPrimitive*>::const_iterator currentIter = items.begin();
    currentIter += items.indexOf(middleItem);

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
        calculateItemPosition(items(), calculateMiddleItem(items(), rect()));
    }
    else
    {
        foreach(FwPrimitive* primitive, items())
        {
            primitive->setX(primitive->x() + step);
        }
    }
}

void FwSlidingFrameLayout::animationStart(FwPrimitive* current)
{
    if(m_animation->state() == QAbstractAnimation::Running)
    {
        if(!m_nextCandidateItem)
        {
            m_animation->setEasingCurve(QEasingCurve::Linear);
            m_nextCandidateItem = current;
        }
        return;
    }

    m_candidateItem = current;
    m_deltaValue = m_candidateItem->x();

    //m_animation->setEasingCurve(QEasingCurve::OutCurve);
    m_animation->setEasingCurve(QEasingCurve::OutBounce);
    m_animation->setStartValue(m_deltaValue);
    m_animation->setEndValue((rect().width() - m_candidateItem->width()) * 0.5);
    m_animation->start();
}

void FwSlidingFrameLayout::animationFinish()
{
    if(m_nextCandidateItem)
    {
        animationStart(m_nextCandidateItem);
        m_nextCandidateItem = 0;
    }
}
