#ifndef FIREWORKS_ITEMLAYOUT_H
#define FIREWORKS_ITEMLAYOUT_H

#include <QtCore/qobject.h>
#include <QtCore/qrect.h>

#include "fireworks.h"

class FwItemView;
class FwPrimitive;
class FwMLObject;

class FwItemLayout : public QObject
{
    Q_OBJECT
    typedef QObject BaseClass;

public:
    FwItemLayout(FwItemView* view);

    inline FwItemView* view() const;

    virtual QByteArray className() const = 0;

    virtual void apply(FwMLObject* object) = 0;

    virtual void init(const QList<FwPrimitive*> items, const QRect& rect) = 0;
    virtual void update(const QList<FwPrimitive*> items, FwPrimitive* current, const QRect& rect) = 0;

    virtual void loop() = 0; //TEMPORARY FUNCTION!!!

    QList<FwPrimitive*> items() const;
    FwPrimitive* currentItem() const;
    QRect rect() const;

protected:
    FwItemView* m_view;
    QRect m_rect;
};

FwItemView* FwItemLayout::view() const
{
    return m_view;
}

///////////////////////////////////////////////////////////////////////

class FwSlidingFrameLayout : public FwItemLayout
{
    Q_OBJECT
    typedef FwItemLayout BaseClass;

public:

    static const char staticClassName[];

    FwSlidingFrameLayout(FwItemView* view);

    QByteArray className() const;
    static FwItemLayout* constructor(FwItemView* view);

    inline Fw::Orientation orientation() const;
    bool setOrientation(Fw::Orientation orientation);

    inline int margin() const;
    void setMargin(int margin);

    void apply(FwMLObject* object);

    void init(const QList<FwPrimitive*> items, const QRect& rect);
    void update(const QList<FwPrimitive*> items, FwPrimitive* current, const QRect& rect);

    void loop();
    void loop(int startX, int endX); //TEMPORARY

protected:
    void timerEvent(QTimerEvent *e);

    FwPrimitive* calculateMiddleItem(const QList<FwPrimitive*>& items, const QRect& rect);

    void move(int delta);

    void calculateItemPosition(const QList<FwPrimitive*>& items);

private:
    Fw::Orientation m_orientation;
    int m_margin;
    int m_criticalPoint;
    int m_startPoint;
    int m_endPoint;
    FwPrimitive* m_middleItem;

    int m_loopTimer; //TEMPORARY!!!

    int m_startX; //TEMPORARY!!!
    int m_endX; //TEMPORARY!!!
    int m_step;//TEMPORARY!!!
    int m_delta; //TEMPORARY!!!
};

Fw::Orientation FwSlidingFrameLayout::orientation() const
{
    return m_orientation;
}

int FwSlidingFrameLayout::margin() const
{
    return m_margin;
}

#endif // FIREWORKS_ITEMLAYOUT_H
