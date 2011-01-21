#include <QtCore/qdebug.h>

#include "fwrectprimitive.h"

#include "fwcore/fwml.h"

#include "fwgui/fwcanvas.h"
#include "fwgui/fwbrush.h"
#include "fwgui/fwgraphicsview.h"
#include "fwgui/fwscene.h"

FwGraphicsRectItem::FwGraphicsRectItem(FwPrimitiveGroup* parent) :
    BaseClass(parent)
{
}

FwGraphicsRectItem::~FwGraphicsRectItem()
{
}

void FwGraphicsRectItem::setBrush(FwBrushPtr brush)
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

void FwGraphicsRectItem::paint(FwCanvas* canvas)
{
    paint(canvas, rect());
}

void FwGraphicsRectItem::paint(FwCanvas* canvas, const QRect& rect)
{
    if(!m_brush.isNull())
    {
        m_brush->drawRect(canvas, rect);
    }
}

QRect FwGraphicsRectItem::updateGeometry(const QRect& rect)
{
    if(m_brush)
    {
        m_brush->setSourceRect(rect);
    }
    return rect;
}

void FwGraphicsRectItem::apply(FwMLObject *object)
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
            qDebug() << "FwGraphicsRectItem::apply(FwMLObject *object)" << bOk;
            if(bOk && !bgColor.isNull())
            {
                setBrush(FwBrushPtr(new FwBrushSolid(bgColor)));
            }
        }
    }

    BaseClass::apply(object);

    update();
}
