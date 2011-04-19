#include <QtCore/qdebug.h>

#include "fwrectprimitive.h"
#include "fwdrawer.h"

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
        prepareGeometryChanged();

        delete m_brush;
        m_brush = brush;
        if(m_brush)
        {
            m_brush->setSourceRect(geometry()->rect());
        }

        update();
    }
}

void FwRectPrimitive::paint(FwPainter* painter, const QRect& clipRect)
{
    if(m_brush)
    {
        m_brush->drawRect(painter, clipRect);
    }
}

void FwRectPrimitive::geometryChangedEvent(const QRect &oldRect, QRect &rect)
{
    Q_UNUSED(oldRect);
    if(m_brush)
    {
        m_brush->setSourceRect(rect);
    }
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

FwDrawer* FwRectPrimitive::createDrawer(FwMLObject *object) const
{
    FwMLNode* radiusNode = object->attribute("radius");
    if(radiusNode)
    {
        bool bOk = false;
        int radius = radiusNode->toInt(&bOk);
        if(bOk)
        {
            return new FwRoundedRectDrawer(radius, this);
        }
    }

    return BaseClass::createDrawer(object);
}
