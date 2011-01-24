#include <QtCore/qdebug.h>

#include "fwrectprimitive.h"

#include "fwcore/fwml.h"

#include "fwgui/fwcanvas.h"
#include "fwgui/fwbrush.h"
#include "fwgui/fwgraphicsview.h"
#include "fwgui/fwscene.h"

FwRectPrimitive::FwRectPrimitive(FwPrimitiveGroup* parent) :
    BaseClass(parent)
{
}

FwRectPrimitive::~FwRectPrimitive()
{
}

void FwRectPrimitive::setBrush(FwBrushPtr brush)
{
    if(m_brush != brush)
    {
        m_brush = brush;
        if(!m_brush.isNull())
        {
            m_brush->setSourceRect(rect());
        }
        invalidate();
    }
}

void FwRectPrimitive::paint(FwCanvas* canvas)
{
    paint(canvas, rect());
}

void FwRectPrimitive::paint(FwCanvas* canvas, const QRect& rect)
{
    if(!m_brush.isNull())
    {
        m_brush->drawRect(canvas, rect);
    }
}

QRect FwRectPrimitive::updateGeometry(const QRect& rect)
{
    if(m_brush)
    {
        m_brush->setSourceRect(rect);
    }
    return rect;
}

void FwRectPrimitive::apply(FwMLObject *object)
{
    prepareGeometryChanged();

    FwMLObject* background = object->attribute("background")->cast<FwMLObject>();
    if(background)
    {
        FwPixmap px = createPixmap(background);
        if(!px.isNull())
        {
            setBrush(FwBrushPtr(new FwBrushTexture(px)));
        }
    }
    else
    {
        FwMLNode* bgColorNode = object->attribute("bgcolor");
        if(bgColorNode)
        {
            bool bOk = false;
            FwColor bgColor = bgColorNode->toColor(&bOk);
            qDebug() << "FwRectPrimitive::apply(FwMLObject *object)" << bOk;
            if(bOk && !bgColor.isNull())
            {
                setBrush(FwBrushPtr(new FwBrushSolid(bgColor)));
            }
        }
    }

    BaseClass::apply(object);

    update();
}
