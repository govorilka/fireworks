#include "fwpixmapbar.h"

#include "fwgui/primitives/fwpixmapprimitive.h"

#include "fwcore/fwml.h"

#include "fwgui/fwpainter.h"
#include "fwgui/fwscene.h"
#include "fwgui/fwgraphicsview.h"
#include "fwgui/fwimagelibrary.h"

FwPixmapBar::FwPixmapBar(const QByteArray& name, FwPrimitiveGroup* parent) :
    BaseClass(name, parent),
    m_leftWidth(0),
    m_centerWidth(0),
    m_rightWidth(0)
{
}

void FwPixmapBar::apply(FwMLObject *object)
{
    prepareGeometryChanged();

    FwMLString* pixmapNode = object->attribute("leftPixmap")->cast<FwMLString>();
    if(pixmapNode && !pixmapNode->isEmpty())
    {
        setLeftPixmap(scene()->view()->imageLibrary()->image(pixmapNode->value()));
    }

    pixmapNode = object->attribute("centerPixmap")->cast<FwMLString>();
    if(pixmapNode && !pixmapNode->isEmpty())
    {
        setCenterPixmap(scene()->view()->imageLibrary()->image(pixmapNode->value()));
    }

    pixmapNode = object->attribute("rightPixmap")->cast<FwMLString>();
    if(pixmapNode && !pixmapNode->isEmpty())
    {
        setRightPixmap(scene()->view()->imageLibrary()->image(pixmapNode->value()));
    }

    BaseClass::apply(object);
    update();
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
