#ifndef FIREWORKS_GRAPHICSITEM_INL_H
#define FIREWORKS_GRAPHICSITEM_INL_H

#include "fwprimitives/fwprimitive.h"

FwPrimitiveGroup* FwPrimitive::parent() const
{
    return m_parent;
}

FwScene* FwPrimitive::scene() const
{
    return m_scene;
}

void FwPrimitive::linkAnchor(FwAnchor* anchor)
{
    if(anchor)
    {
        m_geometry->addAnchor(anchor);
    }
}

Fw::BufferMode FwPrimitive::bufferMode() const
{
    return m_bufferMode;
}

void FwPrimitive::prepareGeometryChanged()
{
    if(!_startChanged)
    {
        invalidate();
    }
    _startChanged++;
}

QPoint FwPrimitive::xy() const
{
    return QPoint(m_x, m_y);
}

void FwPrimitive::setXY(int x, int y)
{
    m_x = x;
    m_y = y;
    if(m_anchor && m_anchor->geometry())
    {
        m_anchor->updateRect(m_anchor->geometry()->rect(), m_geometry->rect());
    }
    else
    {
        setGeometryRect(QRect(QPoint(m_x, m_y), m_geometry->size()));
    }
}

void FwPrimitive::setXY(const QPoint& pos)
{
    setXY(pos.x(), pos.y());
}

int FwPrimitive::x() const
{
    return xy().x();
}

void FwPrimitive::setX(int x)
{
    setXY(x, y());
}

int FwPrimitive::y() const
{
    return xy().y();
}

void FwPrimitive::setY(int y)
{
    setXY(x(), y);
}

QSize FwPrimitive::size() const
{
    return m_geometry->rect().size();
}

void FwPrimitive::setSize(int w, int h)
{
    setSize(QSize(w, h));
}

void FwPrimitive::setSize(const QSize& size)
{
    QRect rect = m_geometry->rect();
    rect.setSize(size);
    if(m_anchor && m_anchor->geometry())
    {
        m_anchor->updateRect(m_anchor->geometry()->rect(),
                             rect);
    }
    else
    {
        setGeometryRect(rect);
    }
}

QRect FwPrimitive::rect() const
{
    return m_geometry->rect();
}

void FwPrimitive::setRect(const QRect& rect)
{
    if(m_anchor && m_anchor->geometry())
    {
        m_anchor->updateRect(m_anchor->geometry()->rect(),
                             rect);
    }
    else
    {
        setGeometryRect(rect);
    }
}

QRect FwPrimitive::boundingRect() const
{
    return m_boundingRect;
}

bool FwPrimitive::isVisible() const
{
    return m_visible;
}

bool FwPrimitive::isVisibleOnScreen() const
{
    return visibleOnScreen;
}

void FwPrimitive::show()
{
    setVisible(true);
}

void FwPrimitive::hide()
{
    setVisible(false);
}

int FwPrimitive::zIndex() const
{
    return m_zIndex;
}

bool FwPrimitive::zIndexLessThan(FwPrimitive* p1, FwPrimitive* p2)
{
    return p1->zIndex() < p2->zIndex();
}

FwAnchor* FwPrimitive::anchor() const
{
    return m_anchor;
}

#endif // FIREWORKS_GRAPHICSITEM_INL_H
