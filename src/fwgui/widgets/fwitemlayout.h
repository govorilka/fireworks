#ifndef FIREWORKS_ITEMLAYOUT_H
#define FIREWORKS_ITEMLAYOUT_H

#include <QtCore/qobject.h>
#include <QtCore/qrect.h>
#include <QtCore/qlist.h>

#include "fireworks.h"

#include "fwgui/widgets/fwanimation.h"

class FwItemView;
class FwPrimitive;
class FwMLObject;
class FwKeyPressEvent;
class FwItemAnimation;

class FwItemLayout : public QObject
{
    Q_OBJECT
    typedef QObject BaseClass;

public:
    friend class FwItemAnimation;

    FwItemLayout(FwItemView* view);

    inline FwItemView* view() const;
    QList<FwPrimitive*> items() const;
    FwPrimitive* currentItem() const;
    QRect rect() const;

    virtual QByteArray className() const = 0;

    virtual void apply(FwMLObject* object);

    virtual void init(const QList<FwPrimitive*> items, FwPrimitive* current, const QRect& rect) = 0;
    virtual void update(const QList<FwPrimitive*>& items, FwPrimitive* current, const QRect& rect) = 0;

    void keyPressEvent(const QList<FwPrimitive*>& items, FwKeyPressEvent* keyEvent);

    void setCurrent(FwPrimitive* previous, FwPrimitive* current, bool visibleOnScreen);

    inline FwItemAnimation* animation() const;
    void setAnimationEnabled(bool enable);

protected:
    virtual FwPrimitive* nextItem(const QList<FwPrimitive*>& items, FwPrimitive* current, FwKeyPressEvent* keyEvent) = 0;

    virtual void animationStart(FwItemAnimation* animation, FwPrimitive *previous, FwPrimitive* current) = 0;
    virtual void animationFinish(FwItemAnimation* animation, FwPrimitive* current);
    virtual void updateAnimationValue(const QVariant& value) = 0;

    void applyCurrentItem(FwPrimitive* current);

    FwItemView* m_view;
    QRect m_rect;

    FwPrimitive* m_candidate;
    FwPrimitive* m_nextCandidate;

    FwItemAnimation* m_animation;

private slots:
    void animationFinish();
};

FwItemView* FwItemLayout::view() const
{
    return m_view;
}

FwItemAnimation* FwItemLayout::animation() const
{
    return m_animation;
}

///////////////////////////////////////////////////////////////////////

class FwItemAnimation : public FwAnimation
{
    Q_OBJECT
    typedef FwAnimation BaseClass;

public:
    FwItemAnimation(FwItemLayout* parent);

    inline FwItemLayout* parent() const;

    inline void restoreCurve();
    inline void resetCurve();

protected:
    void updateCurrentValue(const QVariant & value);

private:
    FwItemLayout* m_parent;
    QEasingCurve m_curve;
};

void FwItemAnimation::restoreCurve()
{
    if(m_curve.type() != QEasingCurve::Linear)
    {
        setEasingCurve(m_curve);
    }
}

void FwItemAnimation::resetCurve()
{
    QEasingCurve curve = easingCurve();
    if(curve.type() != QEasingCurve::Linear)
    {
        m_curve = curve;
        setEasingCurve(QEasingCurve::Linear);
    }
}

///////////////////////////////////////////////////////////////////////

class FwSliderLayout : public FwItemLayout
{
    Q_OBJECT
    typedef FwItemLayout BaseClass;

public:
    FwSliderLayout(FwItemView* view);

    inline int margin() const;
    void setMargin(int margin);

    inline bool isRepeat() const;
    inline void setRepeat(bool repeat);

    void apply(FwMLObject* object);

protected:
    void updateAnimationValue(const QVariant& value);

    FwPrimitive* nextPrimitive(const QList<FwPrimitive*>& items, FwPrimitive* current) const;
    FwPrimitive* prevPrimtive(const QList<FwPrimitive*>& items, FwPrimitive* current) const;

    virtual void applyAnimationStep(int step) = 0;

    int m_margin;
    int m_criticalPoint;
    int m_startPoint;
    int m_endPoint;
    int m_deltaValue;
    int m_maxValue;

private:
    bool m_repeat;
};

int FwSliderLayout::margin() const
{
    return m_margin;
}

bool FwSliderLayout::isRepeat() const
{
    return m_repeat;
}

void FwSliderLayout::setRepeat(bool repeat)
{
    m_repeat = repeat;
}

///////////////////////////////////////////////////////////////////////

class FwHSliderLayout : public FwSliderLayout
{
    Q_OBJECT
    typedef FwSliderLayout BaseClass;

public:

    static const char staticClassName[];

    FwHSliderLayout(FwItemView* view);

    QByteArray className() const;
    static FwItemLayout* constructor(FwItemView* view);

    void init(const QList<FwPrimitive*> items, FwPrimitive* current, const QRect& rect);
    void update(const QList<FwPrimitive*>& items, FwPrimitive* current, const QRect& rect);

protected:
    FwPrimitive* nextItem(const QList<FwPrimitive*>& items, FwPrimitive* current, FwKeyPressEvent* keyEvent);

    void animationStart(FwItemAnimation* animation, FwPrimitive *previous, FwPrimitive* current);

    void applyAnimationStep(int step);
};

///////////////////////////////////////////////////////////////////////

class FwVSliderLayout : public FwSliderLayout
{
    Q_OBJECT
    typedef FwSliderLayout BaseClass;

public:

    static const char staticClassName[];

    FwVSliderLayout(FwItemView* view);

    QByteArray className() const;
    static FwItemLayout* constructor(FwItemView* view);

    void init(const QList<FwPrimitive*> items, FwPrimitive* current, const QRect& rect);
    void update(const QList<FwPrimitive*>& items, FwPrimitive* current, const QRect& rect);

protected:
    FwPrimitive* nextItem(const QList<FwPrimitive*>& items, FwPrimitive* current, FwKeyPressEvent* keyEvent);
    void animationStart(FwItemAnimation* animation, FwPrimitive *previous, FwPrimitive* current);
    void applyAnimationStep(int step);
};


///////////////////////////////////////////////////////////////////////

class FwLoopHSliderLayout : public FwHSliderLayout
{
    Q_OBJECT
    typedef FwHSliderLayout BaseClass;

public:

    static const char staticClassName[];

    FwLoopHSliderLayout(FwItemView* view);

    QByteArray className() const;
    static FwItemLayout* constructor(FwItemView* view);

    void init(const QList<FwPrimitive*> items, FwPrimitive* current, const QRect& rect);
    void update(const QList<FwPrimitive*>& items, FwPrimitive* current, const QRect& rect);

protected:
    void applyAnimationStep(int step);
    void calculatePosition(const QList<FwPrimitive*>& items, FwPrimitive* current);
};

///////////////////////////////////////////////////////////////////////

class FwLoopVSliderLayout : public FwVSliderLayout
{
    Q_OBJECT
    typedef FwVSliderLayout BaseClass;

public:

    static const char staticClassName[];

    FwLoopVSliderLayout(FwItemView* view);

    QByteArray className() const;
    static FwItemLayout* constructor(FwItemView* view);

    void init(const QList<FwPrimitive*> items, FwPrimitive* current, const QRect& rect);
    void update(const QList<FwPrimitive*>& items, FwPrimitive* current, const QRect& rect);

protected:
    void applyAnimationStep(int step);
    void calculatePosition(const QList<FwPrimitive*>& items, FwPrimitive* current);
};

#endif // FIREWORKS_ITEMLAYOUT_H
