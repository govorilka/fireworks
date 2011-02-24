#include <QtCore/qdebug.h>

#include "fwitemlayout.h"
#include "fwitemview.h"

#include"fwcore/fwml.h"

#include "fwgui/fwguievent.h"

FwItemLayout::FwItemLayout(FwItemView* view) :
    BaseClass(view),
    m_view(view),
    m_candidate(0),
    m_nextCandidate(0),
    m_animation(0)
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

void FwItemLayout::keyPressEvent(const QList<FwPrimitive*>& items, FwKeyPressEvent* keyEvent)
{
    if(items.size() > 1)
    {
        FwPrimitive* previous = m_candidate ? m_candidate : m_view->current();
        FwPrimitive* candidatPrimitive = nextItem(items, previous, keyEvent);
        if(candidatPrimitive)
        {
            setCurrent(previous, candidatPrimitive, true);
            keyEvent->accept();
        }
    }
}

void FwItemLayout::setCurrent(FwPrimitive* previous, FwPrimitive* current, bool visibleOnScreen)
{
    if(visibleOnScreen && m_animation)
    {
        if(m_animation->state() == QAbstractAnimation::Running)
        {
            if(!m_nextCandidate)
            {
                m_animation->resetCurve();
                m_nextCandidate = current;
            }
            return;
        }

        m_candidate = current;
        animationStart(m_animation, previous, current);
    }
    else
    {
        applyCurrentItem(current);
        m_view->prepareGeometryChanged();
        update(items(), current, m_view->geometryRect());
        m_view->update();
    }
}

void FwItemLayout::setAnimationEnabled(bool enable)
{
    if(enable && !m_animation)
    {
        m_animation = new FwItemAnimation(this);
        connect(m_animation, SIGNAL(finished()), this, SLOT(animationFinish()));
    }
    else if(!enable && m_animation)
    {
        if(m_animation->state() == QAbstractAnimation::Running)
        {
            m_animation->setCurrentTime(m_animation->duration());
            m_animation->stop();
            animationFinish();
            m_animation->deleteLater();
            m_animation = 0;
        }
    }
}

void FwItemLayout::animationFinish()
{
    if(m_candidate)
    {
        if(m_nextCandidate)
        {
            animationStart(m_animation, m_candidate, m_nextCandidate);
            m_candidate = m_nextCandidate;
            m_nextCandidate = 0;
            return;
        }
        animationFinish(m_animation, m_candidate);
        applyCurrentItem(m_candidate);
    }
}

void FwItemLayout::animationFinish(FwItemAnimation* animation, FwPrimitive* current)
{
    Q_UNUSED(animation);
    Q_UNUSED(current);
}

void FwItemLayout::applyCurrentItem(FwPrimitive* current)
{
    m_candidate = 0;
    m_nextCandidate = 0;
    if(m_animation)
    {
        m_animation->restoreCurve();
    }
    m_view->applyCurrentItem(current);
}

void FwItemLayout::apply(FwMLObject* object)
{
    FwMLObject* animationNode = object->attribute("animation")->cast<FwMLObject>();
    if(animationNode)
    {
        setAnimationEnabled(true);
        if(m_animation)
        {
            m_animation->apply(animationNode);
        }
    }
}

///////////////////////////////////////////////////////////////////////

FwItemAnimation::FwItemAnimation(FwItemLayout* parent) :
    BaseClass(parent),
    m_parent(parent),
    m_curve(QEasingCurve::Linear)
{
}

void FwItemAnimation::updateCurrentValue(const QVariant & value)
{
    m_parent->updateAnimationValue(value);
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
    m_deltaValue(0)
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
    m_criticalPoint = rect.width() + m_margin * (items.size() - 1);
    foreach(FwPrimitive* primitive, items)
    {
        primitive->setY(0.5 * (rect.height() - primitive->height()));
        m_criticalPoint += primitive->width();
    }
    m_criticalPoint = 0.5 * m_criticalPoint;
}

void FwSlidingFrameLayout::update(const QList<FwPrimitive*>& items, FwPrimitive* current, const QRect& rect)
{
    current->setX((rect.width() - current->width()) * 0.5);
    if(items.size() > 1)
    {
        calculatePosition(items, current);
    }
}

void FwSlidingFrameLayout::calculateHPosition(const QList<FwPrimitive*>& items, FwPrimitive* current)
{
    m_startPoint = current->x();
    m_endPoint = m_startPoint + current->width() + m_margin;

    QList<FwPrimitive*>::const_iterator currentIter = items.begin();
    currentIter += items.indexOf(current);

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

void FwSlidingFrameLayout::calculateVPosition(const QList<FwPrimitive*>& items, FwPrimitive* current)
{
    //TODO: !!!
}

FwPrimitive* FwSlidingFrameLayout::nextItem(const QList<FwPrimitive*>& items, FwPrimitive* current, FwKeyPressEvent* keyEvent)
{
    QList<FwPrimitive*>::const_iterator candidateIter = items.begin();

    switch(keyEvent->key())
    {
    case Qt::Key_Right:
        candidateIter += (items.indexOf(current) + 1);
        if(candidateIter == items.end())
        {
            candidateIter = items.begin();
        }
        return (*candidateIter);

    case Qt::Key_Left:
        if(current == (*candidateIter))
        {
            candidateIter = --items.end();
        }
        else
        {
            candidateIter += (items.indexOf(current) - 1);
        }
        return (*candidateIter);

    default:
        break;
    }

    return 0;
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

    BaseClass::apply(object);
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

void FwSlidingFrameLayout::updateAnimationValue(const QVariant& value)
{
    int point = value.toInt();

    int step = point - m_deltaValue;
    m_deltaValue = point;

    m_startPoint += step;
    m_endPoint += step;
    if(m_startPoint > 0 || m_endPoint < rect().width())
    {
        calculatePosition(items(), calculateMiddleItem(items(), rect()));
    }
    else
    {
        foreach(FwPrimitive* primitive, items())
        {
            primitive->setX(primitive->x() + step);
        }
    }
}

void FwSlidingFrameLayout::animationStart(FwItemAnimation* animation, FwPrimitive *previous, FwPrimitive* current)
{
    m_deltaValue = current->x();
    animation->setStartValue(m_deltaValue);
    animation->setEndValue(static_cast<int>((rect().width() - current->width()) * 0.5));
    animation->start();
}
