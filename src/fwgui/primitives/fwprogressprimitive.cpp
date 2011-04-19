#include "fwprogressprimitive.h"

#include "fwgui/fwscene.h"

FwProgressPrimitive::FwProgressPrimitive(const QByteArray& name, FwPrimitiveGroup* parent) :
    BaseClass(name, parent),
    m_minValue(0),
    m_maxValue(0),
    m_value(0),
    m_animation(0),
    m_minProgressWidth(0),
    normalMinProgressWidth(0),
    m_progressRect(new FwGeometry()),
    m_backgroundRect(0, 0, 0, 0),
    m_progressBrush(0)
{
}

FwProgressPrimitive::~FwProgressPrimitive()
{
    delete m_progressBrush;
    if(m_animation)
    {
        m_animation->resetGraphicsItem();
    }
}

void FwProgressPrimitive::setProgressBrush(FwBrush* brush)
{
    if(m_progressBrush != brush)
    {
        prepareGeometryChanged();
        delete m_progressBrush;
        m_progressBrush = brush;
        update();
    }
}

void FwProgressPrimitive::setValue(int min, int max, int value, bool byUser)
{
    if(m_minValue != min || m_maxValue != max || m_value != value)
    {
        m_minValue = min;
        m_maxValue = max;
        m_value = qBound(m_minValue, value, m_maxValue);
        updateProgressRect(byUser);
    }
}

void FwProgressPrimitive::setAnimation(FwProgressAnimation* animation)
{
    if(m_animation != animation)
    {
        if(m_animation)
        {
            m_animation->resetGraphicsItem();
            m_animation = 0;
        }
        if(animation)
        {
            m_animation = animation;
            m_animation->setGraphicsItem(this);
        }
    }
}

void FwProgressPrimitive::geometryChangedEvent(const QRect &oldRect, QRect &rect)
{
    normalMinProgressWidth = qBound(0, m_minProgressWidth, rect.width());
    updateProgressRect(false);
    BaseClass::geometryChangedEvent(oldRect, rect);
}

void FwProgressPrimitive::paint(FwPainter *painter, const QRect &clipRect)
{
    QRect r = clipRect;

    if(m_progressRect->rect().width())
    {
        if(m_progressBrush)
        {
            m_progressBrush->drawRect(painter, m_progressRect->rect().intersect(r));
        }

        r = r.intersected(m_backgroundRect);
    }

    FwBrush* brush = this->brush();
    if(brush && !r.isNull())
    {
        brush->drawRect(painter, r);
    }
}

void FwProgressPrimitive::updateProgressRect(bool byUser)
{
    QRect newProgressRect = rect();
    if(m_value == m_minValue)
    {
        newProgressRect.setWidth(qMax(0., normalMinProgressWidth));
    }
    else if(m_value < m_maxValue)
    {
        qreal width = newProgressRect.width();
        newProgressRect.setWidth(qMax(width / qAbs(m_minValue-m_maxValue)*(m_value-m_minValue),
                                      normalMinProgressWidth));
    }

    if(byUser && m_animation)
    {
        if(m_animation->state() == QAbstractAnimation::Running)
        {
            m_animation->stop();
        }
        m_animation->setStartValue(m_progressRect->rect());
        m_animation->setEndValue(newProgressRect);
        m_animation->start();
    }
    else
    {
        updateProgressRect(newProgressRect);
    }
}

void FwProgressPrimitive::updateProgressRect(const QRect& rect)
{
    if(m_progressRect->rect() != rect)
    {
        prepareGeometryChanged();
        m_progressRect->setRect(rect);
        m_backgroundRect = this->rect().intersected(rect);
        update();
    }
}

void FwProgressPrimitive::setMinProgressWidth(int width)
{
    if(m_minProgressWidth != width)
    {
        m_minProgressWidth = width;
        normalMinProgressWidth = qBound(0, m_minProgressWidth, rect().width());
        updateProgressRect(false);
    }
}

//////////////////////////////////////////////////////////////////////////////////

FwProgressAnimation::FwProgressAnimation(QObject *parent) :
    BaseClass(parent),
    m_item(0)
{
}

FwProgressAnimation::~FwProgressAnimation()
{
    if(m_item)
    {
        m_item->setAnimation(0);
    }
}

void FwProgressAnimation::updateCurrentValue(const QVariant & value)
{
    if(m_item)
    {
        m_item->updateProgressRect(value.toRect());
    }
}

void FwProgressAnimation::updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    if(newState == Stopped && oldState == Running)
    {
        complete();
    }
    BaseClass::updateState(newState, oldState);
}

void FwProgressAnimation::complete()
{
    if(m_item)
    {
        m_item->updateProgressRect(endValue().toRect());
    }
}

void FwProgressAnimation::setGraphicsItem(FwProgressPrimitive * item)
{
    resetGraphicsItem();
    m_item = item;
}

void FwProgressAnimation::resetGraphicsItem()
{
    if(m_item)
    {
        if(state() != QAbstractAnimation::Stopped)
        {
            stop();
            complete();
        }
        m_item = 0;
    }
}
