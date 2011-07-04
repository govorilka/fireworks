#include "fwpixmapbar.h"

#include "fwgui/primitives/fwpixmapprimitive.h"

#include "fwcore/fwml.h"

#include "fwgui/fwpainter.h"


FwPixmapBar::FwPixmapBar(const QByteArray& name, FwPrimitiveGroup* parent) :
    BaseClass(name, parent),
    m_leftWidth(0),
    m_centerWidth(0),
    m_rightWidth(0)
{
}

void FwPixmapBar::apply(FwMLObject *object)
{
    BaseClass::apply(object);

    FwMLNode* pixmapNode = object->attribute("left");
    if(pixmapNode)
    {
        setLeftPixmap(createPixmap(pixmapNode));
    }

    pixmapNode = 0;
    pixmapNode = object->attribute("center");
    if(pixmapNode)
    {
        setCenterPixmap(createPixmap(pixmapNode));
    }

    pixmapNode = 0;
    pixmapNode = object->attribute("right");
    if(pixmapNode)
    {
        setRightPixmap(createPixmap(pixmapNode));
    }
}

void FwPixmapBar::setLeftPixmap(const FwPixmap &value)
{
    if(m_leftPixmap != value)
    {
       prepareGeometryChanged();
       m_leftPixmap = value;
       m_leftWidth = m_leftPixmap.isNull() ? 0 : m_leftPixmap.width();
       update();
    }
}

void FwPixmapBar::setCenterPixmap(const FwPixmap &value)
{
    if(m_centerPixmap != value)
    {
       prepareGeometryChanged();
       m_centerPixmap = value;
       m_centerWidth = m_centerPixmap.isNull() ? 0 : m_centerPixmap.width();
       update();
    }
}

void FwPixmapBar::setRightPixmap(const FwPixmap &value)
{
    if(m_rightPixmap != value)
    {
       prepareGeometryChanged();
       m_rightPixmap = value;
       m_rightWidth = m_rightPixmap.isNull() ? 0 : m_rightPixmap.width();
       update();
    }
}

void FwPixmapBar::paint(FwPainter *painter, const QRect &clipRect)
{
    QRect boundingRect = this->boundingRect();

    if(m_leftWidth && m_leftWidth <= boundingRect.width())
    {
        QRect rect = boundingRect;
        rect.setWidth(m_leftWidth);
        if(clipRect.intersects(rect))
        {
            painter->drawPixmap(rect, m_leftPixmap);
        }
        boundingRect.setLeft(boundingRect.left() + m_leftWidth);
    }

     if(m_rightWidth && boundingRect.width() && m_rightWidth <= boundingRect.width())
     {
         QRect rect = boundingRect;
         rect.setLeft(rect.left() + rect.width() - m_rightWidth);
         if(clipRect.intersects(rect))
         {
             painter->drawPixmap(rect, m_rightPixmap);
         }
         boundingRect.setRight(boundingRect.right() - m_rightWidth);
     }

     if(boundingRect.width() > 0 && m_centerWidth && clipRect.intersects(boundingRect))
     {
         painter->drawPixmap(boundingRect, m_centerPixmap);
     }
}
