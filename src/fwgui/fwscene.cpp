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

FwScene::FwScene(const QByteArray& name, FwGraphicsView* view) :
    QObject(view),
    BaseClass(name, 0),
    m_view(view),
    m_messageBox(0),
    m_messageBoxTemplate(0),
    m_messageBoxAllow(true),
    m_darkBackground(0)
{
    setObjectName(QString::fromUtf8(name));

    setChildrenClipRect(true);

    m_scene = this;
    m_visible = true;
    visibleOnScreen = true;

    Q_ASSERT(!m_view->scene(name));
    m_view->m_scenes.append(this);
    if(!m_view->m_activeScene)
    {
        m_view->setActiveScene(this);
    }
}

FwScene::~FwScene()
{
    removeItems();
    if(m_view)
    {
        if(m_view->m_activeScene == this)
        {
            m_view->setActiveScene(0);
        }
        m_view->m_scenes.removeAll(this);
        m_view = 0;
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

        case Fw::E_Resize:
            resizeEvent(static_cast<FwResizeEvent*>(e));
            return true;

        default:
            break;
        }
    }

    return QObject::event(e);
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
    m_view->m_dirtyRegion->pushObjectRect(m_geometry->rect());
    if(m_contentDirty)
    {
        if(m_geometry->isDirty())
        {
            invalidateGeometry();
        }
        m_view->m_dirtyRegion->addChildrenRect(m_geometry->rect());
        m_contentDirty = false;
    }
    invalidateChildren();
    m_view->m_dirtyRegion->popObjectRect();
}

void FwScene::apply(FwMLObject *object)
{
    FwMLObject* temp = object->attribute("messagebox")->cast<FwMLObject>();
    if(temp)
    {
        setMessageBoxTemplate(static_cast<FwMLObject*>(temp->clone()));
    }

    FwMLBool* messageboxAllow = object->attribute("messageBoxAllow")->cast<FwMLBool>();
    if(messageboxAllow)
    {
        setMessageBoxAllow(messageboxAllow->value());
    }

    FwMLObject* darkBackgroundNode = object->attribute("darkBackgroundColor")->cast<FwMLObject>();
    if(darkBackgroundNode)
    {
        FwBrush* brush = createBrush(darkBackgroundNode);
        if(brush)
        {
            m_darkBackground = new FwRectPrimitive("darkBackground", this);
            m_darkBackground->prepareGeometryChanged();
            m_darkBackground->setPosition(Fw::HP_CenterDock, Fw::VP_MiddleDock);
            m_darkBackground->setBrush(brush);
            m_darkBackground->setZIndex(1000); //STUB!!!
            m_darkBackground->setIgnoreParentMargin(true);
            m_darkBackground->hide();
            m_darkBackground->update();
        }
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

bool FwScene::showMessageBox(const FwRequest& request)
{
    if(!request.isNull())
    {
        if(!m_messageBox)
        {
            m_messageBox = createMessageBox(m_messageBoxTemplate);
            m_messageBox->setRequest(request);
            m_messageBox->show();
            if(m_darkBackground)
            {
                m_darkBackground->show();
            }
            return true;
        }
    }
    return false;
}

FwMessageBox* FwScene::createMessageBox(FwMLObject* messageBoxTemplate)
{
    FwMessageBox* box = new FwMessageBox("messagebox", this);
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
    else
    {
        keyPressEvent(event);
    }
    return event->isAccepted();
}

void FwScene::cleanup()
{
    removeItems();
}

void FwScene::keyPressEvent(FwKeyPressEvent* event)
{
    Q_UNUSED(event);
}

void FwScene::resizeEvent(FwResizeEvent* event)
{
    Q_UNUSED(event);
}

