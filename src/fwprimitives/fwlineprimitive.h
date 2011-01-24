#ifndef FIREWORKS_GRAPHICSLINEITEM_H
#define FIREWORKS_GRAPHICSLINEITEM_H

#include <QtCore/qline.h>

#include "fireworks.h"

#include "fwgui/fwpen.h"

#include "fwprimitives/fwprimitive.h"

class FwLinePrimitive;

typedef FwLinePrimitive* FwLinePrimitivePtr;

class FwLinePrimitive : public FwPrimitive
{
    typedef FwPrimitive BaseClass;

public:
    FwLinePrimitive(FwPrimitiveGroup* parent);

    inline FwPenPtr pen() const;
    inline void setPen(const FwPenPtr& pen);

    inline QLine line() const;
    inline void setLine(int x1, int y1, int x2, int y2);
    void setLine(const QLine& line);

    inline int orientation() const;
    inline int length() const;

    void apply(FwMLObject *object);

protected:
    virtual QRect updateGeometry(const QRect& rect);

    virtual void paint(FwCanvas* canvas);

private:
    FwPenPtr m_pen;
    QPoint m_p2;
    int m_lenght;
    int m_orientation;
};

#include "fwprimitives/fwlineprimitive_inl.h"

#endif // FIREWORKS_GRAPHICSLINEITEM_H
