#ifndef FIREWORKS_GRAPHICSITEMGROUP_H
#define FIREWORKS_GRAPHICSITEMGROUP_H

#include <QtCore/qvector.h>
#include <QtCore/qlist.h>

#include "fwprimitives/fwrectprimitive.h"

class FwPrimitiveGroup : public FwRectPrimitive
{
    typedef FwRectPrimitive BaseClass;

public:
    friend class FwPrimitive;
    friend class FwScene;

    FwPrimitiveGroup(const QByteArray& name, FwPrimitiveGroup* parent);
    virtual ~FwPrimitiveGroup();

    inline QVector<FwPrimitive*> primitives() const;

    void removeItems();

    void apply(FwMLObject *object);

protected:
    inline void sortZIndex();

    QRect updateGeometry(const QRect& rect);

    void paint(FwPainter *painter, const QRect &clipRect);

    virtual void visibleChangedEvent();

private:
    QVector<FwPrimitive*> m_primitives;
};

#include "fwprimitives/fwprimitivegroup_inl.h"

#endif //FIREWORKS_GRAPHICSITEMGROUP_H
