#ifndef FIREWORKS_SCENE_INL_H
#define FIREWORKS_SCENE_INL_H

#include "fwgui/fwscene.h"

int FwScene::id() const
{
    return m_id;
}

FwGraphicsView* FwScene::view() const
{
    return m_view;
}

void FwScene::invalidate()
{
    m_dirtyRegion = QRegion(rect());
    invalidateDirtyRegion();
}

////////////////////////////////////////////////////////////////////////////////


FwScene* FwSceneShowEvent::previous() const
{
    return m_previous;
}

////////////////////////////////////////////////////////////////////////////////

FwScene* FwSceneHideEvent::next() const
{
    return m_next;
}

#endif // FIREWORKS_SCENE_INL_H
