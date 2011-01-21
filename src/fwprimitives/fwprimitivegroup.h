#ifndef FIREWORKS_GRAPHICSITEMGROUP_H
#define FIREWORKS_GRAPHICSITEMGROUP_H

#include <QtCore/qvector.h>
#include <QtCore/qlist.h>

#include "fwprimitives/fwrectprimitive.h"

class FwPrimitiveGroup : public FwGraphicsRectItem
{
    typedef FwGraphicsRectItem BaseClass;

public:
    friend class FwPrimitive;
    friend class FwScene;

    FwPrimitiveGroup(FwPrimitiveGroup* parent);
    virtual ~FwPrimitiveGroup();

    inline QVector<FwPrimitive*> items() const;

    void removeItems();

protected:
    inline void sortZIndex();

    QRect updateGeometry(const QRect& rect);

    void paint(FwCanvas* canvas);

    virtual void visibleChangedEvent();

private:
    QVector<FwPrimitive*> m_items;
};

#include "fwprimitives/fwprimitivegroup_inl.h"

#endif //FIREWORKS_GRAPHICSITEMGROUP_H
