#ifndef FIREWORKS_GRAPHICSVIEW_INL_H
#define FIREWORKS_GRAPHICSVIEW_INL_H

#include <QtCore/qcoreapplication.h>

#include "fwgui/fwgraphicsview.h"

QSize FwGraphicsView::size() const
{
    return m_size;
}

FwScene* FwGraphicsView::activeScene() const
{
    return m_activeScene;
}

void FwGraphicsView::setActiveScene(const QByteArray& name)
{
    setActiveScene(scene(name));
}

QRect FwGraphicsView::rect() const
{
    return QRect(QPoint(0, 0), size());
}

void FwGraphicsView::update()
{
    update(rect());
}

void FwGraphicsView::update(const QRect& rect)
{
    m_dirtyRegion->addRect(rect);
    if(!m_dirtyRegion->isEmpty() && m_needPostUpdateEvent)
    {
        postUpdateEvent();
    }
}

void FwGraphicsView::postUpdateEvent()
{
    if(m_needPostUpdateEvent)
    {
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateLater));
        m_needPostUpdateEvent = false;
    }
}

FwImageLibrary* FwGraphicsView::imageLibrary() const
{
    return m_imageLibrary;
}

FwScene* FwGraphicsView::prevActiveScene() const
{
    return m_prevActiveScene;
}

#endif // FIREWORKS_GRAPHICSVIEW_INL_H
