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
    m_view->startChangedCurrent();

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
        if(!m_animation->isRunning())
        {
            applyCurrentItem(current);;
            updateCurrentPos(current);
        }
    }
    else
    {
        applyCurrentItem(current);
        updateCurrentPos(current);
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

FwPrimitive* FwItemLayout::nextItem(const QList<FwPrimitive*>& items, FwPrimitive* current, FwKeyPressEvent* keyEvent)
{
    Q_UNUSED(current);
    switch(keyEvent->key())
    {
    case Qt::Key_Home:
        return items.first();

    case Qt::Key_End:
        return items.back();

    default:
        break;
    }
    return 0;
}

void FwItemLayout::initItemsPos(const QList<FwPrimitive*> items, FwPrimitive* current)
{
    init(items, m_view->geometryRect());
    updateCurrentPos(current);
}

void FwItemLayout::updateCurrentPos(FwPrimitive* current)
{
    update(m_view->items(), current, m_view->geometryRect());
    FwPrimitive* highlight = m_view->highlight();
    if(highlight)
    {
        updateHighlightPos(highlight, current, m_view->geometryRect());
    }
}

///////////////////////////////////////////////////////////////////////

FwItemAnimation::FwItemAnimation(FwItemLayout* parent) :
    BaseClass(parent),
    m_parent(parent)
{
}

void FwItemAnimation::updateCurrentValue(const QVariant & value)
{
    m_parent->updateAnimationValue(value);
}

///////////////////////////////////////////////////////////////////////

FwSliderLayout::FwSliderLayout(FwItemView* view) :
    BaseClass(view),
    m_margin(0),
    m_criticalPoint(0),
    m_startPoint(0),
    m_endPoint(0),
    m_deltaValue(0),
    m_maxValue(0),
    m_repeat(false)
{
}

void FwSliderLayout::setMargin(int margin)
{
    if(m_margin != margin)
    {
        m_view->prepareGeometryChanged();
        m_margin = margin;
        m_view->updateChildrenRect();
        m_view->update();
    }
}

void FwSliderLayout::apply(FwMLObject* object)
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

    FwMLNode* repeatNode = object->attribute("repeat");
    if(repeatNode)
    {
        bool bOk = false;
        bool repeat = repeatNode->toBool(&bOk);
        if(bOk)
        {
            setRepeat(repeat);
        }
    }

    BaseClass::apply(object);
}

void FwSliderLayout::updateAnimationValue(const QVariant& value)
{
    int point = value.toInt();

    int step = point - m_deltaValue;
    m_deltaValue = point;

    m_startPoint += step;
    m_endPoint += step;

    applyAnimationStep(step);
}

FwPrimitive* FwSliderLayout::nextPrimitive(const QList<FwPrimitive*>& items, FwPrimitive* current) const
{
    QList<FwPrimitive*>::const_iterator candidateIter = items.begin();
    candidateIter += (items.indexOf(current) + 1);
    if(candidateIter == items.end())
    {
        if(!m_repeat)
        {
            return 0;
        }
        candidateIter = items.begin();
    }
    return (*candidateIter);
}

FwPrimitive* FwSliderLayout::prevPrimtive(const QList<FwPrimitive*>& items, FwPrimitive* current) const
{
    QList<FwPrimitive*>::const_iterator candidateIter = items.begin();
    if(current == (*candidateIter))
    {
        if(!m_repeat)
        {
            return 0;
        }
        candidateIter = --items.end();
    }
    else
    {
        candidateIter += (items.indexOf(current) - 1);
    }
    return (*candidateIter);
}

void FwSliderLayout::updateHighlightPos(FwPrimitive* highlight, FwPrimitive* currentItem, const QRect& rect)
{
    Q_UNUSED(highlight);
    Q_UNUSED(currentItem);
    Q_UNUSED(rect);
}

///////////////////////////////////////////////////////////////////////

const char FwHSliderLayout::staticClassName[] = "fireworks.layouts.HorizontalSlider";

FwHSliderLayout::FwHSliderLayout(FwItemView* view) :
    BaseClass(view)
{
}

QByteArray FwHSliderLayout::className() const
{
    return staticClassName;
}

FwItemLayout* FwHSliderLayout::constructor(FwItemView* view)
{
    return new FwHSliderLayout(view);
}

void FwHSliderLayout::init(const QList<FwPrimitive*> items, const QRect& rect)
{
    foreach(FwPrimitive* primitive, items)
    {
        primitive->setY(0.5 * (rect.height() - primitive->height()));
    }
}

void FwHSliderLayout::update(const QList<FwPrimitive*>& items, FwPrimitive* current, const QRect& rect)
{
    current->setX((rect.width() - current->width()) * 0.5);
    if(items.size() > 1)
    {
        m_startPoint = current->x();
        m_endPoint = m_startPoint + current->width() + m_margin;

        QList<FwPrimitive*>::const_iterator currentIter = items.begin();
        currentIter += items.indexOf(current);

        QList<FwPrimitive*>::const_iterator beginIter = items.begin();
        QList<FwPrimitive*>::const_iterator rightIter = currentIter + 1;
        while(rightIter != items.end())
        {
            FwPrimitive* primitive = (*rightIter);
            primitive->setX(m_endPoint);
            m_endPoint += (primitive->width() + m_margin);
            ++rightIter;
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
    }
}

FwPrimitive* FwHSliderLayout::nextItem(const QList<FwPrimitive*>& items, FwPrimitive* current, FwKeyPressEvent* keyEvent)
{
    switch(keyEvent->key())
    {
    case Qt::Key_Right:
        return nextPrimitive(items, current);

    case Qt::Key_Left:
        return prevPrimtive(items, current);

    default:
        break;
    }
    return BaseClass::nextItem(items, current, keyEvent);
}

void FwHSliderLayout::applyAnimationStep(int step)
{
    foreach(FwPrimitive* primitive, m_view->items())
    {
        primitive->setX(primitive->x() + step);
    }
}

void FwHSliderLayout::animationStart(FwItemAnimation* animation, FwPrimitive *previous, FwPrimitive* current)
{
    Q_UNUSED(previous);
    m_deltaValue = current->x();
    animation->setStartValue(m_deltaValue);
    animation->setEndValue(static_cast<int>((m_view->rect().width() - current->width()) * 0.5));
    animation->start();
}

///////////////////////////////////////////////////////////////////////

const char FwVSliderLayout::staticClassName[] = "fireworks.layouts.VerticalSlider";

FwVSliderLayout::FwVSliderLayout(FwItemView* view) :
    BaseClass(view)
{
}

QByteArray FwVSliderLayout::className() const
{
    return staticClassName;
}

FwItemLayout* FwVSliderLayout::constructor(FwItemView* view)
{
    return new FwVSliderLayout(view);
}

void FwVSliderLayout::init(const QList<FwPrimitive*> items, const QRect& rect)
{
    foreach(FwPrimitive* primitive, items)
    {
        primitive->setX(0.5 * (rect.width() - primitive->width()));
    }
}

void FwVSliderLayout::update(const QList<FwPrimitive*>& items, FwPrimitive* current, const QRect& rect)
{
    current->setY((rect.height() - current->height()) * 0.5);
    if(items.size() > 1)
    {
        m_startPoint = current->y();
        m_endPoint = m_startPoint + current->height() + m_margin;

        QList<FwPrimitive*>::const_iterator currentIter = items.begin();
        currentIter += items.indexOf(current);

        QList<FwPrimitive*>::const_iterator beginIter = items.begin();
        QList<FwPrimitive*>::const_iterator rightIter = currentIter + 1;

        while(rightIter != items.end())
        {
            FwPrimitive* primitive = (*rightIter);
            primitive->setY(m_endPoint);
            m_endPoint += (primitive->height() + m_margin);
            ++rightIter;
        }

        if(beginIter != currentIter)
        {
            QList<FwPrimitive*>::const_iterator leftIter = currentIter;
            do
            {
                --leftIter;
                FwPrimitive* primitive = (*leftIter);
                primitive->setY(m_startPoint -= (primitive->height() + m_margin));
            }
            while(leftIter != beginIter);
        }
    }
}

FwPrimitive* FwVSliderLayout::nextItem(const QList<FwPrimitive*>& items, FwPrimitive* current, FwKeyPressEvent* keyEvent)
{
    switch(keyEvent->key())
    {
    case Qt::Key_Up:
        return nextPrimitive(items, current);

    case Qt::Key_Down:
        return prevPrimtive(items, current);

    default:
        break;
    }
    return BaseClass::nextItem(items, current, keyEvent);
}

void FwVSliderLayout::applyAnimationStep(int step)
{
    foreach(FwPrimitive* primitive, m_view->items())
    {
        primitive->setY(primitive->y() + step);
    }
}

void FwVSliderLayout::animationStart(FwItemAnimation* animation, FwPrimitive *previous, FwPrimitive* current)
{
    Q_UNUSED(previous);
    m_deltaValue = current->y();
    animation->setStartValue(m_deltaValue);
    animation->setEndValue(static_cast<int>((m_view->rect().height() - current->height()) * 0.5));
    animation->start();
}

///////////////////////////////////////////////////////////////////////

const char FwLoopHSliderLayout::staticClassName[] = "fireworks.layouts.LoopHorizontalSlider";

FwLoopHSliderLayout::FwLoopHSliderLayout(FwItemView* view) :
    BaseClass(view)
{
    setRepeat(true);
}

QByteArray FwLoopHSliderLayout::className() const
{
    return staticClassName;
}

FwItemLayout* FwLoopHSliderLayout::constructor(FwItemView* view)
{
    return new FwLoopHSliderLayout(view);
}

void FwLoopHSliderLayout::init(const QList<FwPrimitive*> items, const QRect& rect)
{
    m_maxValue = rect.width();
    m_criticalPoint = m_maxValue + m_margin * (items.size() - 1);
    foreach(FwPrimitive* primitive, items)
    {
        primitive->setY(0.5 * (rect.height() - primitive->height()));
        m_criticalPoint += primitive->width();
    }
    m_criticalPoint = 0.5 * m_criticalPoint;
}

void FwLoopHSliderLayout::update(const QList<FwPrimitive*>& items, FwPrimitive* current, const QRect& rect)
{
    current->setX((rect.width() - current->width()) * 0.5);
    if(items.size() > 1)
    {
        calculatePosition(items, current);
    }
}

void FwLoopHSliderLayout::calculatePosition(const QList<FwPrimitive*>& items, FwPrimitive* current)
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

void FwLoopHSliderLayout::applyAnimationStep(int step)
{
    if(m_startPoint > 0 || m_endPoint < m_maxValue)
    {
        QList<FwPrimitive*> items = m_view->items();
        FwPrimitive* middleItem = 0;
        int halfWidth = m_maxValue * 0.5;
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
        calculatePosition(items, middleItem);
    }
    else
    {
        BaseClass::applyAnimationStep(step);
    }
}

///////////////////////////////////////////////////////////////////////

const char FwLoopVSliderLayout::staticClassName[] = "fireworks.layouts.LoopVerticalSlider";

FwLoopVSliderLayout::FwLoopVSliderLayout(FwItemView* view) :
    BaseClass(view)
{
    setRepeat(true);
}

QByteArray FwLoopVSliderLayout::className() const
{
    return staticClassName;
}

FwItemLayout* FwLoopVSliderLayout::constructor(FwItemView* view)
{
    return new FwLoopVSliderLayout(view);
}

void FwLoopVSliderLayout::init(const QList<FwPrimitive*> items, const QRect& rect)
{
    m_maxValue = rect.height();
    m_criticalPoint = m_maxValue + m_margin * (items.size() - 1);
    foreach(FwPrimitive* primitive, items)
    {
        primitive->setX(0.5 * (rect.width() - primitive->width()));
        m_criticalPoint += primitive->height();
    }
    m_criticalPoint = 0.5 * m_criticalPoint;
}

void FwLoopVSliderLayout::update(const QList<FwPrimitive*>& items, FwPrimitive* current, const QRect& rect)
{
    current->setY((rect.height() - current->height()) * 0.5);
    if(items.size() > 1)
    {
        calculatePosition(items, current);
    }
}

void FwLoopVSliderLayout::calculatePosition(const QList<FwPrimitive*>& items, FwPrimitive* current)
{
    m_startPoint = current->y();
    m_endPoint = m_startPoint + current->height() + m_margin;

    QList<FwPrimitive*>::const_iterator currentIter = items.begin();
    currentIter += items.indexOf(current);

    QList<FwPrimitive*>::const_iterator beginIter = items.begin();
    QList<FwPrimitive*>::const_iterator rightIter = currentIter + 1;

    while(rightIter != items.end() && m_endPoint <= m_criticalPoint)
    {
        FwPrimitive* primitive = (*rightIter);
        primitive->setY(m_endPoint);
        m_endPoint += (primitive->height() + m_margin);
        ++rightIter;
    }
    if(rightIter == items.end())
    {
        while(beginIter != currentIter && m_endPoint <= m_criticalPoint)
        {
            FwPrimitive* primitive = (*beginIter);
            primitive->setY(m_endPoint);
            m_endPoint += (primitive->height() + m_margin);
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
            primitive->setY(m_startPoint -= (primitive->height() + m_margin));
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
            primitive->setY(m_startPoint -= (primitive->height() + m_margin));
        }
        while(leftIter != rightIter);
    }
}

void FwLoopVSliderLayout::applyAnimationStep(int step)
{
    if(m_startPoint > 0 || m_endPoint < m_maxValue)
    {
        QList<FwPrimitive*> items = m_view->items();
        FwPrimitive* middleItem = 0;
        int halfHeight = m_maxValue * 0.5;
        int delta = halfHeight;
        foreach(FwPrimitive* primitive, items)
        {
            int newDelta = qMin(qAbs(halfHeight - primitive->geometryRect().bottom()),
                                qAbs(primitive->geometryRect().top() - halfHeight));
            if(newDelta < delta)
            {
                delta = newDelta;
                middleItem = primitive;
            }
        }
        calculatePosition(items, middleItem);
    }
    else
    {
        BaseClass::applyAnimationStep(step);
    }
}

///////////////////////////////////////////////////////////////////////

const char FwPagesLayout::staticClassName[] = "fireworks.layouts.Pages";

FwPagesLayout::FwPagesLayout(FwItemView* view) :
    BaseClass(view),
    m_currentPage(-1)
{
    setRepeat(true);
}

QByteArray FwPagesLayout::className() const
{
    return staticClassName;
}

FwItemLayout* FwPagesLayout::constructor(FwItemView* view)
{
    return new FwPagesLayout(view);
}

void FwPagesLayout::init(const QList<FwPrimitive*> items, const QRect& rect)
{
    m_maxValue = rect.height();

    m_pageIndex.clear();
    m_pages.clear();

    int y = 0;
    m_currentPage = 0;
    foreach(FwPrimitive* primitive, items)
    {
        if(y != 0 && (y + primitive->height()) >= m_maxValue)
        {
            y = 0;
            ++m_currentPage;
        }

        m_pageIndex.insert(primitive, m_currentPage);
        m_pages.insert(m_currentPage, primitive);

        primitive->prepareGeometryChanged();
        primitive->setPos(0, y);
        primitive->setVisible(false);
        primitive->update();

        y += (primitive->height() + m_margin);
    }
    m_currentPage = -1;
}

void FwPagesLayout::update(const QList<FwPrimitive*>& items, FwPrimitive* current, const QRect& rect)
{
    Q_UNUSED(rect);
    calculatePosition(items, current);
}

void FwPagesLayout::animationStart(FwItemAnimation* animation, FwPrimitive *previous, FwPrimitive* current)
{
    Q_UNUSED(animation);
    Q_UNUSED(previous);
    calculatePosition(m_view->items(), current);

    FwPrimitive* highlight = m_view->highlight();
    if(highlight)
    {
        animation->setStartValue(highlight->rect());
        animation->setEndValue(highlightRect(current, highlight->rect()));
        animation->start();
    }
}

void FwPagesLayout::calculatePosition(const QList<FwPrimitive*>& items, FwPrimitive* current)
{
    int nextPage = m_pageIndex.value(current, -1);
    if(m_currentPage != nextPage)
    {
        if(m_currentPage != -1)
        {
            foreach(FwPrimitive* primitive, m_pages.values(m_currentPage))
            {
                primitive->setVisible(false);
            }
        }
        if(nextPage != -1)
        {
            foreach(FwPrimitive* primitive, m_pages.values(nextPage))
            {
                primitive->setVisible(true);
            }
            m_currentPage = nextPage;
        }
    }
}

void FwPagesLayout::updateAnimationValue(const QVariant &value)
{
    FwPrimitive* highlight = m_view->highlight();
    if(highlight)
    {
        highlight->setRect(value.toRect());
    }
}

void FwPagesLayout::applyAnimationStep(int step)
{
    Q_UNUSED(step);
}

FwPrimitive* FwPagesLayout::nextItem(const QList<FwPrimitive *> &items, FwPrimitive *current, FwKeyPressEvent *keyEvent)
{
    switch(keyEvent->key())
    {
    case Qt::Key_Up:
        return prevPrimtive(items, current);

    case Qt::Key_Down:
        return nextPrimitive(items, current);

    default:
        break;
    }
    return BaseClass::nextItem(items, current, keyEvent);
}

void FwPagesLayout::updateHighlightPos(FwPrimitive* highlight, FwPrimitive* currentItem, const QRect& rect)
{
    highlight->setRect(highlightRect(currentItem, QRect(0, 0, rect.width(), highlight->height())));
}

QRect FwPagesLayout::highlightRect(FwPrimitive* current, const QRect& currentRect) const
{
    QRect rect = currentRect;
    rect.setY(current->y());
    rect.setHeight(current->height());
    return rect;
}
