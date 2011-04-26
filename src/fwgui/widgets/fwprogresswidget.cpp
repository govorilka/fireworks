#include "fwprogresswidget.h"

#include "fwcore/fwml.h"

#include "fwgui/fwscene.h"

FwProgressWidget::FwProgressWidget(const QByteArray& name, FwPrimitiveGroup* parent) :
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

FwProgressWidget::~FwProgressWidget()
{
    delete m_progressBrush;
    if(m_animation)
    {
        m_animation->resetGraphicsItem();
    }
}

void FwProgressWidget::setProgressBrush(FwBrush* brush)
{
    if(m_progressBrush != brush)
    {
        prepareGeometryChanged();
        delete m_progressBrush;
        m_progressBrush = brush;
        update();
    }
}

void FwProgressWidget::setValue(int min, int max, int value, bool byUser)
{
    if(m_minValue != min || m_maxValue != max || m_value != value)
    {
        m_minValue = min;
        m_maxValue = max;
        m_value = qBound(m_minValue, value, m_maxValue);
        updateValueDisplay(geometry()->rect(), byUser);
    }
}

void FwProgressWidget::setAnimation(FwProgressAnimation* animation)
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

void FwProgressWidget::geometryChangedEvent(const QRect &oldRect, QRect &rect)
{
    BaseClass::geometryChangedEvent(oldRect, rect);

    normalMinProgressWidth = qBound(0, m_minProgressWidth, rect.width());
    updateValueDisplay(rect, false);
}

void FwProgressWidget::paint(FwPainter *painter, const QRect &clipRect)
{
    if(m_progressRect->rect().width() && m_progressBrush)
    {
        m_progressBrush->drawRect(painter, clipRect);
    }

    BaseClass::paint(painter, m_backgroundRect.intersect(clipRect));
}

void FwProgressWidget::updateValueDisplay(const QRect& rect, bool byUser)
{
    QRect newProgressRect = rect;
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
        updateProgressRect(rect, newProgressRect);
    }
}

void FwProgressWidget::updateProgressRect(const QRect& geometryRect, const QRect& progressRect)
{
    if(m_progressRect->rect() != progressRect)
    {
        prepareGeometryChanged();

        m_progressRect->setRect(progressRect);
        if(m_progressBrush)
        {
            m_progressBrush->setSourceRect(progressRect);
        }

        m_backgroundRect = geometryRect.adjusted(progressRect.width(), 0, 0, 0);

        update();
    }
}

void FwProgressWidget::setMinProgressWidth(int width)
{
    if(m_minProgressWidth != width)
    {
        m_minProgressWidth = width;
        normalMinProgressWidth = qBound(0, m_minProgressWidth, rect().width());
        updateValueDisplay(geometry()->rect(), false);
    }
}

void FwProgressWidget::apply(FwMLObject *object)
{
    prepareGeometryChanged();

    FwMLObject* progressBrushNode = object->attribute("progress")->cast<FwMLObject>();
    if(progressBrushNode)
    {
        FwBrush* progressBrush = createBrush(progressBrushNode);
        if(progressBrush)
        {
            setProgressBrush(progressBrush);
        }
    }

    FwMLObject* progressAnimationNode = object->attribute("animation")->cast<FwMLObject>();
    if(progressAnimationNode)
    {
        if(!m_animation)
        {
            setAnimation(new FwProgressAnimation(this));
        }
        m_animation->apply(progressAnimationNode);
    }

    BaseClass::apply(object);

    update();
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
        m_item->updateProgressRect(m_item->geometry()->rect(), value.toRect());
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
        m_item->updateProgressRect(m_item->geometry()->rect(), endValue().toRect());
    }
}

void FwProgressAnimation::setGraphicsItem(FwProgressWidget * item)
{
    if(m_item != item)
    {
        resetGraphicsItem();
        m_item = item;
    }
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

