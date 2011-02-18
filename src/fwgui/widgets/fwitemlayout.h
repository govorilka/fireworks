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

    virtual void init(const QList<FwPrimitive*> items, FwPrimitive* current, const QRect& rect) = 0;
    virtual void update(const QList<FwPrimitive*> items, FwPrimitive* current, const QRect& rect) = 0;

protected:
    FwItemView* m_view;
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

    void init(const QList<FwPrimitive*> items, FwPrimitive* current, const QRect& rect);
    void update(const QList<FwPrimitive*> items, FwPrimitive* current, const QRect& rect);

private:
    Fw::Orientation m_orientation;
    int m_margin;
    int m_criticalPoint;
    FwPrimitive* m_middleItem;
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
