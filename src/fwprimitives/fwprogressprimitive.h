#ifndef FIREWORKS_GRAPHICSPROGRESSITEM_H
#define FIREWORKS_GRAPHICSPROGRESSITEM_H

#include <QtCore/qvariantanimation.h>

#include "fwprimitives/fwrectprimitive.h"

class FwGraphicsProgressAnimation;

class FwGraphicsProgressItem : public FwRectPrimitive
{
    typedef FwRectPrimitive BaseClass;

public:
    friend class FwGraphicsProgressAnimation;

    FwGraphicsProgressItem(const QByteArray& name, FwPrimitiveGroup* parent);
    ~FwGraphicsProgressItem();

    inline FwBrushPtr progressBrush() const;
    void setProgressBrush(const FwBrushPtr& brush);

    inline int minValue() const;
    inline int maxValue() const;
    inline void setRange(int min, int max);

    inline int value() const;
    inline void setValue(int value, bool byUser = false);
    inline void resetValue(bool byUser = false);

    void setValue(int min, int max, int value, bool byUser = false);

    inline FwGraphicsProgressAnimation* animation() const;
    void setAnimation(FwGraphicsProgressAnimation* animation);

    inline int minProgressWidth() const;
    void setMinProgressWidth(int width);

    inline void linkProgressRect(FwAnchor* anchor);

protected:
    QRect updateGeometry(const QRect &rect);

    void paint(FwPainter *painter, const QRect &clipRect);

    void updateProgressRect(bool byUser);
    void updateProgressRect(const QRect& rect);

private:
    FwBrushPtr m_progressBrush;

    FwGraphicsProgressAnimation* m_animation;

    int m_minValue;
    int m_maxValue;
    int m_value;

    int m_minProgressWidth;
    qreal normalMinProgressWidth;
    FwGeometry* m_progressRect;
    QRect m_backgroundRect;
};

FwBrushPtr FwGraphicsProgressItem::progressBrush() const
{
    return m_progressBrush;
}

int FwGraphicsProgressItem::minValue() const
{
    return m_minValue;
}

int FwGraphicsProgressItem::maxValue() const
{
    return m_maxValue;
}

void FwGraphicsProgressItem::setRange(int min, int max)
{
    setValue(min, max, m_value, false);
}

int FwGraphicsProgressItem::value() const
{
    return m_value;
}

void FwGraphicsProgressItem::setValue(int value, bool byUser)
{
    setValue(m_minValue, m_maxValue, value, byUser);
}

void FwGraphicsProgressItem::resetValue(bool byUser)
{
    setValue(0, 0, 0, byUser);
}

FwGraphicsProgressAnimation* FwGraphicsProgressItem::animation() const
{
    return m_animation;
}

void FwGraphicsProgressItem::linkProgressRect(FwAnchor* anchor)
{
    if(anchor && anchor != this->anchor())
    {
        m_progressRect->addAnchor(anchor);
    }
}

//////////////////////////////////////////////////////////////////////////////////

class FwGraphicsProgressAnimation : public QVariantAnimation
{
    Q_OBJECT
    typedef QVariantAnimation BaseClass;

public:
    friend class FwGraphicsProgressItem;

    FwGraphicsProgressAnimation(QObject* parent = 0);
    ~FwGraphicsProgressAnimation();

protected:
    void updateCurrentValue(const QVariant & value);
    void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);

    void complete();

    void updatePrimitive();

private:
    void setGraphicsItem(FwGraphicsProgressItem * item);
    void resetGraphicsItem();

    FwGraphicsProgressItem * m_item;
};

#endif // FIREWORKS_GRAPHICSPROGRESSITEM_H
