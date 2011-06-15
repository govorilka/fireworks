#include <QtCore/qcoreevent.h>
#include <QtCore/qcoreapplication.h>
#include <QtCore/qdebug.h>
#include <QtCore/qcoreevent.h>

#include "fwscene.h"
#include "fwgraphicsview.h"
#include "fwguievent.h"

#include "fwgui/widgets/fwwidget.h"

FwScene::FwScene(int id, FwGraphicsView* view) :
    BaseClass("scene", 0),
    m_view(view),
    m_id(id)
{
    visibleOnScreen = true;

    setParent(m_view);

    Q_ASSERT(!m_view->scene(id));
    m_view->m_scenes.append(this);
    m_scene = this;
}

FwScene::~FwScene()
{
    if(m_view)
    {
        if(m_view->m_activeScene == this)
        {
            m_view->setActiveScene(0);
        }
        m_view->m_scenes.removeAll(this);
    }
}

void FwScene::showEvent(FwSceneShowEvent* event)
{
    Q_UNUSED(event);
    setVisible(true);
}

void FwScene::hideEvent(FwSceneHideEvent* event)
{
    Q_UNUSED(event);
    setVisible(false);
}

void FwScene::showAnimationFinished()
{

}

void FwScene::hideAnimationFinished()
{
}

bool FwScene::event(QEvent * e)
{
    if(e->type() == FwGuiEvent::qtTypeID())
    {
        FwGuiEvent* fwEvent = static_cast<FwGuiEvent*>(e);
        switch(fwEvent->eventType())
        {
        case Fw::E_SceneShow:
            showEvent(static_cast<FwSceneShowEvent*>(fwEvent));
            return true;

        case Fw::E_SceneHide:
            hideEvent(static_cast<FwSceneHideEvent*>(e));
            return true;

        default:
            break;
        }
    }

    return BaseClass::event(e);
}

FwFont FwScene::font(const FwFontDescription& description)
{
    return m_view->font(description);
}

void FwScene::showEventProcessed(FwSceneShowEvent* e)
{
    setVisible(true);
    showEvent(e);
}

void FwScene::hideEventProcessed(FwSceneHideEvent* e)
{
    setVisible(false);
    hideEvent(e);
}

bool FwScene::isActive() const
{
    return m_view && m_view->m_activeScene == this;
}

void FwScene::invalidate()
{
    m_view->m_dirtyRegion.setObjectRect(m_geometry->rect());
    if(m_contentDirty)
    {
        if(m_geometry->isDirty())
        {
            invalidateGeometry();
        }
        m_view->m_dirtyRegion.addChildrenRect(m_geometry->rect());
        m_contentDirty = false;
    }
    invalidateChildren();
}
