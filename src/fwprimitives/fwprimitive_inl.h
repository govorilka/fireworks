#ifndef FIREWORKS_GRAPHICSITEM_INL_H
#define FIREWORKS_GRAPHICSITEM_INL_H

#include "fwprimitives/fwprimitive.h"
#include "fwprimitives/fwgeometry.h"

FwPrimitiveGroup* FwPrimitive::parent() const
{
    return m_parent;
}

FwScene* FwPrimitive::scene() const
{
    return m_scene;
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

QPoint FwPrimitive::pos() const
{
    return m_pos;
}

void FwPrimitive::setPos(int x, int y)
{
    setPos(QPoint(x, y));
}

int FwPrimitive::x() const
{
    return m_pos.x();
}

void FwPrimitive::setX(int x)
{
    setPos(x, m_pos.y());
}

int FwPrimitive::y() const
{
    return m_pos.y();
}

void FwPrimitive::setY(int y)
{
    setPos(m_pos.x(), y);
}

QSize FwPrimitive::size() const
{
    return m_geometry->size();
}

void FwPrimitive::setSize(int w, int h)
{
    setSize(QSize(w, h));
}

int FwPrimitive::width() const
{
    return m_geometry->size().width();
}

void FwPrimitive::setWidth(int width)
{
    setSize(width, height());
}

int FwPrimitive::height() const
{
    return m_geometry->size().height();
}

void FwPrimitive::setHeight(int height)
{
    setSize(width(), height);
}

QRect FwPrimitive::rect() const
{
    return QRect(m_pos, m_geometry->size());
}

FwGeometry* FwPrimitive::geometry() const
{
    return m_geometry;
}

QRect FwPrimitive::geometryRect() const
{
    return m_geometry->rect();
}

void FwPrimitive::setGeometryRect(const QRect& rect)
{
    if(m_geometry->rect() != rect)
    {
        prepareGeometryChanged();
        m_geometry->setRect(rect);
        update();
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

QByteArray FwPrimitive::name() const
{
    return m_name;
}

void FwPrimitive::setName(const QByteArray& name)
{
    m_name = name;
}

bool FwPrimitive::isLinked() const
{
    return m_parentGeometry;
}

Fw::HorizontalPosition FwPrimitive::hPosition() const
{
    return m_hPosition;
}

void FwPrimitive::setHPosition(Fw::HorizontalPosition position)
{
    setPosition(position, m_vPosition);
}

Fw::VerticalPosition FwPrimitive::vPosition() const
{
    return m_vPosition;
}

void FwPrimitive::setVPosition(Fw::VerticalPosition position)
{
    setPosition(m_hPosition, position);
}

#endif // FIREWORKS_GRAPHICSITEM_INL_H
