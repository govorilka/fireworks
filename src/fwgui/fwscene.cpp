#include <QtCore/qcoreevent.h>
#include <QtCore/qcoreapplication.h>
#include <QtCore/qdebug.h>
#include <QtCore/qcoreevent.h>

#include "fwscene.h"
#include "fwgraphicsview.h"
#include "fwguievent.h"

#include "fwgui/widgets/fwmessagebox.h"

#include "fwgui/widgets/fwwidget.h"

#include "fwcore/fwml.h"

FwScene::FwScene(int id, FwGraphicsView* view) :
    BaseClass("scene", 0),
    m_view(view),
    m_id(id),
    m_messageBox(0),
    m_messageBoxTemplate(0)
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
        case Fw::E_KeyPress:
            return keyEventProccessed(static_cast<FwKeyPressEvent*>(fwEvent));

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
    m_view->m_dirtyRegion.pushObjectRect(m_geometry->rect());
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
    m_view->m_dirtyRegion.popObjectRect();
}

void FwScene::apply(FwMLObject *object)
{
    FwMLObject* temp = object->attribute("messagebox")->cast<FwMLObject>();
    if(temp)
    {
        setMessageBoxTemplate(static_cast<FwMLObject*>(temp->clone()));
    }

    BaseClass::apply(object);
}

void FwScene::setMessageBoxTemplate(FwMLObject* temp)
{
    if(m_messageBoxTemplate != temp)
    {
        delete m_messageBoxTemplate;
        m_messageBoxTemplate = temp;
        if(m_messageBox)
        {
            m_messageBox->apply(m_messageBoxTemplate);
        }
    }
}

void FwScene::postRequest(const FwRequest& request)
{
    if(!request.isNull())
    {
        if(!m_messageBox)
        {
            m_messageBox = createMessageBox(m_messageBoxTemplate);
            m_messageBox->show();
        }
        m_messageBox->setRequest(request);
    }
}

FwMessageBox* FwScene::createMessageBox(FwMLObject* messageBoxTemplate)
{
    FwMessageBox* box = new FwMessageBox("messagebox", this);
    FwRectPrimitive* button = new FwRectPrimitive("buttonOk", box);
    new FwStringPrimitive("caption", box);
    if(messageBoxTemplate)
    {
        box->apply(messageBoxTemplate);
    }
    return box;
}

bool FwScene::keyEventProccessed(FwKeyPressEvent* event)
{
    if(m_messageBox)
    {
        QCoreApplication::sendEvent(m_messageBox, event);
        return true;
    }

    return BaseClass::event(event);
}
