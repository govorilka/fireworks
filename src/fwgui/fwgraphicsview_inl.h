#ifndef FIREWORKS_GRAPHICSVIEW_INL_H
#define FIREWORKS_GRAPHICSVIEW_INL_H

#include "fwgui/fwgraphicsview.h"

QSize FwGraphicsView::size() const
{
    return m_size;
}

void FwGraphicsView::setActiveScene(int id)
{
    setActiveScene(scene(id));
}

QRect FwGraphicsView::rect() const
{
    return QRect(QPoint(0, 0), size());
}

#endif // FIREWORKS_GRAPHICSVIEW_INL_H
