#include <QtCore/qcoreevent.h>
#include <QtCore/qcoreapplication.h>
#include <QtCore/qdebug.h>

#include <QtGui/qevent.h>

#include "fwscene.h"
#include "fwgraphicsview.h"

#include "fwgui/widgets/fwwidget.h"

FwScene::FwScene(int id, FwGraphicsView* view) :
    BaseClass("scene", 0),
    m_view(view),
    m_id(id)
{
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

    foreach(FwWidget* widget, m_widgets)
    {
        delete widget;
    }

    m_widgets.clear();
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
    if(e->type() == FwSceneShowEvent::typeID())
    {
        FwSceneShowEvent* showEvent = static_cast<FwSceneShowEvent*>(e);
        showEventProcessed(showEvent);
        e->accept();
        return true;
    }
    else if(e->type() == FwSceneHideEvent::typeID())
    {
        FwSceneHideEvent* hideEvent = static_cast<FwSceneHideEvent*>(e);
        hideEventProcessed(hideEvent);
        e->accept();
        return true;
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

void FwScene::paint(FwPainter *painter, const QRect &clipRect)
{
    invalidateChildren();
    BaseClass::paint(painter, clipRect);
}

void FwScene::updateCanvas(const QRect& rect)
{
    if(m_view && m_view->m_activeScene == this)
    {
        m_view->updateCanvas(rect);
    }
}

////////////////////////////////////////////////////////////////////////////////

FwSceneShowEvent::FwSceneShowEvent(FwScene* previous) :
    BaseClass(static_cast<QEvent::Type>(typeID())),
    m_previous(previous)
{
}

int FwSceneShowEvent::typeID()
{
    static int _typeID = QEvent::registerEventType();
    return _typeID;
}

////////////////////////////////////////////////////////////////////////////////

FwSceneHideEvent::FwSceneHideEvent(FwScene* next) :
    BaseClass(static_cast<QEvent::Type>(typeID())),
    m_next(next)
{
}

int FwSceneHideEvent::typeID()
{
    static int _typeID = QEvent::registerEventType();
    return _typeID;
}
