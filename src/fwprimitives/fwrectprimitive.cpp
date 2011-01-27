#include <QtCore/qdebug.h>

#include "fwrectprimitive.h"

#include "fwcore/fwml.h"

#include "fwgui/fwpainter.h"
#include "fwgui/fwbrush.h"
#include "fwgui/fwgraphicsview.h"
#include "fwgui/fwscene.h"

FwRectPrimitive::FwRectPrimitive(const QByteArray& name, FwPrimitiveGroup* parent) :
    BaseClass(name, parent),
    m_brush(0)
{
}

FwRectPrimitive::~FwRectPrimitive()
{
    delete m_brush;
}

void FwRectPrimitive::setBrush(FwBrush* brush)
{
    if(m_brush != brush)
    {
        delete m_brush;
        m_brush = brush;
        if(m_brush)
        {
            m_brush->setSourceRect(rect());
        }
        invalidate();
    }
}

void FwRectPrimitive::paint(FwPainter* painter, const QRect& clipRect)
{
    if(m_brush)
    {
        m_brush->drawRect(painter, clipRect);
    }
}

QRect FwRectPrimitive::updateGeometry(const QRect& rect)
{
    if(m_brush)
    {
        m_brush->setSourceRect(rect);
        return rect;
    }   
    return QRect(rect.topLeft(), rect.topLeft());
}

void FwRectPrimitive::apply(FwMLObject *object)
{
    prepareGeometryChanged();

    FwBrush* brush = createBrush(object);
    if(brush)
    {
        setBrush(brush);
    }

    BaseClass::apply(object);

    update();
}
