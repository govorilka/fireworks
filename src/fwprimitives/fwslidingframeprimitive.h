#ifndef FIREWORKS_SLIDINGFRAMEPRIMITIVE_H
#define FIREWORKS_SLIDINGFRAMEPRIMITIVE_H

#include <QtCore/qlist.h>

#include "fwprimitives/fwprimitivegroup.h"

class FwSlidingFramePrimitive : public FwPrimitiveGroup
{
    typedef FwPrimitiveGroup BaseClass;

public:

    FwSlidingFramePrimitive(const QByteArray& name, FwPrimitiveGroup* parent);
    ~FwSlidingFramePrimitive();

    inline QList<FwPrimitive*> items() const;

    void addItem(FwPrimitive* primitive);

    inline FwPrimitive* current() const;
    void setCurrent(FwPrimitive* primitive);

protected:
    QRect updateGeometry(const QRect &rect);

    virtual void updateLayout(const QList<FwPrimitive*> items, FwPrimitive* current);

private:
    QList<FwPrimitive*> m_items;
    FwPrimitive* m_current;

    int m_itemMargin;
};

QList<FwPrimitive*> FwSlidingFramePrimitive::items() const
{
    return m_items;
}

FwPrimitive* FwSlidingFramePrimitive::current() const
{
    return m_current;
}

#endif // FIREWORKS_SLIDINGFRAMEPRIMITIVE_H
