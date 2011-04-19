#ifndef FIREWORKS_GRAPHICSPROGRESSITEM_H
#define FIREWORKS_GRAPHICSPROGRESSITEM_H

#include <QtCore/qvariantanimation.h>

#include "fwgui/primitives/fwrectprimitive.h"

class FwProgressAnimation;

class FwProgressPrimitive : public FwRectPrimitive
{
    typedef FwRectPrimitive BaseClass;

public:
    friend class FwProgressAnimation;

    FwProgressPrimitive(const QByteArray& name, FwPrimitiveGroup* parent);
    ~FwProgressPrimitive();

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

protected:
    void geometryChangedEvent(const QRect &oldRect, QRect &rect);

    void paint(FwPainter *painter, const QRect &clipRect);

    void updateProgressRect(bool byUser);
    void updateProgressRect(const QRect& rect);

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

FwBrush* FwProgressPrimitive::progressBrush() const
{
    return m_progressBrush;
}

int FwProgressPrimitive::minValue() const
{
    return m_minValue;
}

int FwProgressPrimitive::maxValue() const
{
    return m_maxValue;
}

void FwProgressPrimitive::setRange(int min, int max)
{
    setValue(min, max, m_value, false);
}

int FwProgressPrimitive::value() const
{
    return m_value;
}

void FwProgressPrimitive::setValue(int value, bool byUser)
{
    setValue(m_minValue, m_maxValue, value, byUser);
}

void FwProgressPrimitive::resetValue(bool byUser)
{
    setValue(0, 0, 0, byUser);
}

FwProgressAnimation* FwProgressPrimitive::animation() const
{
    return m_animation;
}

FwGeometry* FwProgressPrimitive::progressRect() const
{
    return m_progressRect;
}

//////////////////////////////////////////////////////////////////////////////////

class FwProgressAnimation : public QVariantAnimation
{
    Q_OBJECT
    typedef QVariantAnimation BaseClass;

public:
    friend class FwProgressPrimitive;

    FwProgressAnimation(QObject* parent = 0);
    ~FwProgressAnimation();

protected:
    void updateCurrentValue(const QVariant & value);
    void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);

    void complete();

    void updatePrimitive();

private:
    void setGraphicsItem(FwProgressPrimitive * item);
    void resetGraphicsItem();

    FwProgressPrimitive * m_item;
};

#endif // FIREWORKS_GRAPHICSPROGRESSITEM_H
