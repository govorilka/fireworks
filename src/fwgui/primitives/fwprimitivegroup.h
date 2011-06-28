#ifndef FIREWORKS_GRAPHICSITEMGROUP_H
#define FIREWORKS_GRAPHICSITEMGROUP_H

#include <QtCore/qvector.h>
#include <QtCore/qlist.h>

#include "fwgui/primitives/fwrectprimitive.h"

class FwGraphicsObject;

class FIREWORKSSHARED_EXPORT FwPrimitiveGroup : public FwRectPrimitive
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

    inline void updateChildren();

    void invalidateChildren();

    FwPrimitive* primitiveByName(const QList<QByteArray>& name, int firstElement = 0);

protected:
    void paint(FwPainter *painter, const QRect &clipRect);

    virtual void visibleChangedEvent();
    virtual void childrenRectChangedEvent(bool posChanged, bool sizeChanged);

    void invalidateChildrenRect();

    bool childrenDirty;

    bool needSortZIndex;

private:
    QVector<FwPrimitive*> m_primitives;
    QList<FwPrimitive*> m_visiblePrimitives;
    QList<FwPrimitiveGroup*> m_groups;

    QRect m_childrenRect;

    bool m_invalidateChildrenRect;
};

#include "fwgui/primitives/fwprimitivegroup_inl.h"

#endif //FIREWORKS_GRAPHICSITEMGROUP_H
