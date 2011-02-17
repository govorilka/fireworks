#ifndef FIREWORKS_GRAPHICSITEMGROUP_H
#define FIREWORKS_GRAPHICSITEMGROUP_H

#include <QtCore/qvector.h>
#include <QtCore/qlist.h>

#include "fwgui/primitives/fwrectprimitive.h"

class FwGraphicsObject;

class FwPrimitiveGroup : public FwRectPrimitive
{
    typedef FwRectPrimitive BaseClass;

public:
    friend class FwGraphicsObject;
    friend class FwPrimitive;
    friend class FwScene;

    FwPrimitiveGroup(const QByteArray& name, FwPrimitiveGroup* parent);
    virtual ~FwPrimitiveGroup();

    inline QVector<FwPrimitive*> primitives() const;

    void removeItems();

    void apply(FwMLObject *object);

    virtual FwGraphicsObject* object() const;

    inline void updateChildRect();

protected:
    inline void sortZIndex();

    void paint(FwPainter *painter, const QRect &clipRect);

    virtual void visibleChangedEvent();

    virtual void invalidateChildrenRect();

private:
    QVector<FwPrimitive*> m_primitives;
    QList<FwPrimitiveGroup*> m_groups;

    QRect m_childrenRect;
    bool m_childrenRectDirty;
};

#include "fwgui/primitives/fwprimitivegroup_inl.h"

#endif //FIREWORKS_GRAPHICSITEMGROUP_H
