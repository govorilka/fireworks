#ifndef FWPROGRESSWIDGET_H
#define FWPROGRESSWIDGET_H

#include <QtCore/qvariantanimation.h>

#include "fwgui/widgets/fwwidget.h"
#include "fwgui/widgets/fwanimation.h"

class FwProgressAnimation;

class FIREWORKSSHARED_EXPORT FwProgressWidget : public FwWidget
{
    Q_OBJECT
    typedef FwWidget BaseClass;

public:
    friend class FwProgressAnimation;

    FwProgressWidget(const QByteArray& name, FwPrimitiveGroup* parent);
    ~FwProgressWidget();

    inline FwBrush* progressBrush() const;
    void setProgressBrush(FwBrush* brush);

    inline int minValue() const;
    inline int maxValue() const;
    inline void setRange(int min, int max);

    inline int value() const;
    inline void setValue(int value, bool byUser = false);
    inline void resetValue(bool byUser = false);

    void setValue(int min, int max, int value, bool byUser = false);

    inline FwProgressAnimation* animation() const;
    void setAnimation(FwProgressAnimation* animation);

    inline int minProgressWidth() const;
    void setMinProgressWidth(int width);

    inline FwGeometry* progressRect() const;

    virtual void apply(FwMLObject *object);

protected:
    void geometryChangedEvent(const QRect &oldRect, QRect &rect);

    void paint(FwPainter *painter, const QRect &clipRect);

    void updateValueDisplay(const QRect& rect, bool byUser);
    void updateProgressRect(const QRect& geometryRect, const QRect& progressRect);

private:
    FwBrush* m_progressBrush;

    FwProgressAnimation* m_animation;

    int m_minValue;
    int m_maxValue;
    int m_value;

    int m_minProgressWidth;
    qreal normalMinProgressWidth;
    FwGeometry* m_progressRect;
    QRect m_backgroundRect;
};

FwBrush* FwProgressWidget::progressBrush() const
{
    return m_progressBrush;
}

int FwProgressWidget::minValue() const
{
    return m_minValue;
}

int FwProgressWidget::maxValue() const
{
    return m_maxValue;
}

void FwProgressWidget::setRange(int min, int max)
{
    setValue(min, max, m_value, false);
}

int FwProgressWidget::value() const
{
    return m_value;
}

void FwProgressWidget::setValue(int value, bool byUser)
{
    setValue(m_minValue, m_maxValue, value, byUser);
}

void FwProgressWidget::resetValue(bool byUser)
{
    setValue(0, 0, 0, byUser);
}

FwProgressAnimation* FwProgressWidget::animation() const
{
    return m_animation;
}

FwGeometry* FwProgressWidget::progressRect() const
{
    return m_progressRect;
}

//////////////////////////////////////////////////////////////////////////////////

class FwProgressAnimation : public FwAnimation
{
    Q_OBJECT
    typedef FwAnimation BaseClass;

public:
    friend class FwProgressWidget;

    FwProgressAnimation(QObject* parent = 0);
    ~FwProgressAnimation();

protected:
    void updateCurrentValue(const QVariant & value);
    void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);

    void complete();

    void updatePrimitive();

private:
    void setGraphicsItem(FwProgressWidget * item);
    void resetGraphicsItem();

    FwProgressWidget * m_item;
};

#endif // FWPROGRESSWIDGET_H
