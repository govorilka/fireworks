#include "fwprogressprimitive.h"

#include "fwgui/fwscene.h"

FwGraphicsProgressItem::FwGraphicsProgressItem(const QByteArray& name, FwPrimitiveGroup* parent) :
    BaseClass(name, parent),
    m_minValue(0),
    m_maxValue(0),
    m_value(0),
    m_animation(0),
    m_minProgressWidth(0),
    normalMinProgressWidth(0),
    m_progressRect(new FwGeometry()),
    m_backgroundRect(0, 0, 0, 0)
{
}

FwGraphicsProgressItem::~FwGraphicsProgressItem()
{
    if(m_animation)
    {
        m_animation->resetGraphicsItem();
    }
}

void FwGraphicsProgressItem::setProgressBrush(const FwBrushPtr& brush)
{
    if(m_progressBrush != brush)
    {
        m_progressBrush = brush;
        invalidate();
    }
}

void FwGraphicsProgressItem::setValue(int min, int max, int value, bool byUser)
{
    if(m_minValue != min || m_maxValue != max || m_value != value)
    {
        m_minValue = min;
        m_maxValue = max;
        m_value = qBound(m_minValue, value, m_maxValue);
        updateProgressRect(byUser);
    }
}

void FwGraphicsProgressItem::setAnimation(FwGraphicsProgressAnimation* animation)
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

QRect FwGraphicsProgressItem::updateGeometry(const QRect &rect)
{
    normalMinProgressWidth = qBound(0, m_minProgressWidth, rect.width());
    updateProgressRect(false);
    return BaseClass::updateGeometry(rect);
}

void FwGraphicsProgressItem::paint(FwPainter *painter, const QRect &clipRect)
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

void FwGraphicsProgressItem::updateProgressRect(bool byUser)
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

void FwGraphicsProgressItem::updateProgressRect(const QRect& rect)
{
    if(m_progressRect->rect() != rect)
    {
        m_progressRect->setRect(rect);
        m_backgroundRect = this->rect().intersected(rect);
        invalidate();
    }
}

void FwGraphicsProgressItem::setMinProgressWidth(int width)
{
    if(m_minProgressWidth != width)
    {
        m_minProgressWidth = width;
        normalMinProgressWidth = qBound(0, m_minProgressWidth, rect().width());
        updateProgressRect(false);
    }
}

//////////////////////////////////////////////////////////////////////////////////

FwGraphicsProgressAnimation::FwGraphicsProgressAnimation(QObject *parent) :
    BaseClass(parent),
    m_item(0)
{
}

FwGraphicsProgressAnimation::~FwGraphicsProgressAnimation()
{
    if(m_item)
    {
        m_item->setAnimation(0);
    }
}

void FwGraphicsProgressAnimation::updateCurrentValue(const QVariant & value)
{
    if(m_item)
    {
        m_item->updateProgressRect(value.toRect());
    }
}

void FwGraphicsProgressAnimation::updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    if(newState == Stopped && oldState == Running)
    {
        complete();
    }
    BaseClass::updateState(newState, oldState);
}

void FwGraphicsProgressAnimation::complete()
{
    if(m_item)
    {
        m_item->updateProgressRect(endValue().toRect());
    }
}

void FwGraphicsProgressAnimation::setGraphicsItem(FwGraphicsProgressItem * item)
{
    resetGraphicsItem();
    m_item = item;
}

void FwGraphicsProgressAnimation::resetGraphicsItem()
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
