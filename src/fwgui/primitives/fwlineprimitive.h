#ifndef FIREWORKS_GRAPHICSLINEITEM_H
#define FIREWORKS_GRAPHICSLINEITEM_H

#include <QtCore/qline.h>

#include "fwgui/primitives/fwprimitive.h"

class FwPen;

class FwLinePrimitive : public FwPrimitive
{
    typedef FwPrimitive BaseClass;

public:
    FwLinePrimitive(const QByteArray& name, FwPrimitiveGroup* parent);
    ~FwLinePrimitive();

    inline FwPen* pen() const;
    void setPen(FwPen* pen);

    inline QLine line() const;
    inline void setLine(int x1, int y1, int x2, int y2);
    void setLine(const QLine& line);

    inline int orientation() const;
    inline int length() const;

    void apply(FwMLObject *object);

protected:
    void geometryChanged(const QRect& oldRect, QRect& rect);

    virtual void paint(FwPainter *painter, const QRect &clipRect);

    virtual void penChangedEvent(FwPen* pen);

private:
    FwPen* m_pen;
    QPoint m_p2;
    int m_lenght;
    int m_orientation;
};

#include "fwgui/primitives/fwlineprimitive_inl.h"

#endif // FIREWORKS_GRAPHICSLINEITEM_H