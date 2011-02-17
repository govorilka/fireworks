#ifndef FIREWORKS_SLIDINGFRAMEPRIMITIVE_H
#define FIREWORKS_SLIDINGFRAMEPRIMITIVE_H

#include <QtCore/qlist.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>

#include "fwgui/primitives/fwprimitivegroup.h"

class FwMLObject;
class FwStringPrimitive;

class FwSlidingFramePrimitive : public FwPrimitiveGroup
{
    typedef FwPrimitiveGroup BaseClass;

public:

    FwSlidingFramePrimitive(const QByteArray& name, FwPrimitiveGroup* parent);
    ~FwSlidingFramePrimitive();

    inline QList<FwPrimitive*> items() const;

    void addItem(FwPrimitive* primitive, const QVariant& data = QVariant());
    FwStringPrimitive* addItem(const QString& text, const QVariant& data = QVariant());

    inline FwPrimitive* current() const;
    void setCurrent(FwPrimitive* primitive);

    inline int itemMargin() const;
    void setItemMargin(int margin);

    inline FwMLObject* itemTemplate() const;
    void setItemTemplate(FwMLObject* itemTemplate);

    void apply(FwMLObject *object);

protected:
    void geometryChanged(const QRect &oldRect, QRect &rect);
    void invalidateChildrenRect();

    virtual void updateLayout(const QList<FwPrimitive*> items, FwPrimitive* current, const QRect& rect);

private:
    QList<FwPrimitive*> m_items;
    FwPrimitive* m_current;

    int m_itemMargin;
    FwMLObject* m_itemTemplate;
};

QList<FwPrimitive*> FwSlidingFramePrimitive::items() const
{
    return m_items;
}

FwPrimitive* FwSlidingFramePrimitive::current() const
{
    return m_current;
}

int FwSlidingFramePrimitive::itemMargin() const
{
    return m_itemMargin;
}

FwMLObject* FwSlidingFramePrimitive::itemTemplate() const
{
    return m_itemTemplate;
}

#endif // FIREWORKS_SLIDINGFRAMEPRIMITIVE_H
