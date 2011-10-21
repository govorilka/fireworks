#include <QtCore/qpropertyanimation.h>
#include <QtCore/qcoreapplication.h>
#include <QtCore/qdebug.h>

#include "fwcore/fwml.h"

#include "fwwidget.h"

#include "fwgui/fwscene.h"
#include "fwgui/fwguievent.h"

#include "fwutils/fwrequest.h"

/*!
\class FwWidget
Класс реализует различные элементы пользовательского интерфейса.
*/
FwWidget::FwWidget(const QByteArray& name, FwPrimitiveGroup* parent) :
    QObject(),
    BaseClass(name, parent),
    m_showAnimation(0),
    m_hideAnimation(0),
    m_oldSize(0, 0),
    m_visibleTime(0),
    m_visibleTimerId(0),
    m_darkBackground(false)
{
    setParent(m_scene);
    m_scene->m_widgets.append(this);

    m_visible = false;
    visibleOnScreen = false;

    setChildrenClipRect(false);
}

FwWidget::~FwWidget()
{
    if(m_parent && m_scene)
    {
        m_scene->m_widgets.removeAll(this);
    }
}

/*!
\fn void FwWidget::setVisibleTime(int msec)
Устанавливает время в миллисекундах, сколько окно должно отображаться на экране.
\param msec Время в миллисекундах, которое окно должно находится на экране.
\note Если окно отображается на экране, по вызова функции, будет запущен таймер
сокрытия окна.
\note Если необходимо, чтобы находлось на экране бесконечно долго, установите
время равное 0.
\sa void FwWidget::stopVisibleTimer()
\sa void FwWidget::startVisibleTimer()
*/

/*!
Останавливает таймер сокрытия окна
\sa void FwWidget::setVisibleTime(int msec)
\sa void FwWidget::startVisibleTimer()
*/
void FwWidget::stopVisibleTimer()
{
    if(m_visibleTimerId)
    {
        killTimer(m_visibleTimerId);
        m_visibleTimerId = 0;
    }
}

/*!
Запускает таймер сокрытия окна.
\note Если таймер уже запущен, она его останавливает и запускает заново.
\sa void FwWidget::setVisibleTime(int msec)
\sa void FwWidget::stopVisibleTimer()
*/
void FwWidget::startVisibleTimer()
{
    stopVisibleTimer();
    if(isVisibleOnScreen() && m_visibleTime)
    {
        m_visibleTimerId = startTimer(m_visibleTime);
    }
}

bool FwWidget::event(QEvent * e)
{
    if(e->type() == FwGuiEvent::qtTypeID())
    {
        FwGuiEvent* fwEvent = static_cast<FwGuiEvent*>(e);
        switch(fwEvent->eventType())
        {
        case Fw::E_Show:
            showEvent(static_cast<FwShowEvent*>(fwEvent));
            showAnimationFinished();
            return true;

        case Fw::E_Hide:
            hideEvent(static_cast<FwHideEvent*>(e));
            stopVisibleTimer();
            return true;

        case Fw::E_KeyPress:
            keyPressEvent(static_cast<FwKeyPressEvent*>(fwEvent));
            return true;

        case Fw::E_Resize:
            resizeEvent(static_cast<FwResizeEvent*>(e));
            return true;

        default:
            break;
        }
    }
    else if(e->type() == FwResult::typeID())
    {
        requestAcceptEvent(static_cast<FwResult*>(e));
        return true;
    }

    switch(e->type())
    {
    case QEvent::FontChange:
        e->accept();
        fontChangedEvent(font());
        return true;

    default:
        break;
    }

    return QObject::event(e);
}

//void FwWidget::showEventProcessed(QShowEvent* event)
//{
//    /*if(m_showAnimation)
//    {
//        if(m_showAnimation->state() == QPropertyAnimation::Stopped)
//        {
//            setProperty(m_showAnimation->propertyName(), m_showAnimation->startValue());
//            m_showAnimation->start();
//        }
//    }
//    else
//    {
//        showAnimationFinished();
//    }

//    showEvent(event);

//    if(visibleOnShell())
//    {
//        m_sceneManager->setActiveWidget(this);
//    }

//    updateCanvas(QRegion(m_rect));*/
//}

//void FwWidget::hideEventProcessed(QHideEvent* event)
//{
//    hideEvent(event);

//    /*if(m_showAnimation)
//    {
//        m_showAnimation->stop();
//    }

//    stopVisibleTimer();

//    if(m_sceneManager)
//    {
//        m_sceneManager->removeFromStack(this);
//    }

//    updateCanvas(QRegion(m_rect));*/
//}

void FwWidget::timerEvent(QTimerEvent* event)
{
    if(m_visibleTimerId == event->timerId())
    {
        hide();
    }
}

void FwWidget::showEvent(FwShowEvent* event)
{
    event->accept();
}

void FwWidget::hideEvent(FwHideEvent* event)
{
    event->accept();
}

void FwWidget::fontChangedEvent(const FwFont& font)
{
    Q_UNUSED(font);
}

void FwWidget::showAnimationFinished()
{
    prepareGeometryChanged();
    startVisibleTimer();
    update();
}

void FwWidget::hideAnimationFinished()
{
}

void FwWidget::visibleChangedEvent()
{
    BaseClass::visibleChangedEvent();
    if(m_scene->isActive())
    {
        if(isVisible())
        {
            FwShowEvent event(isVisibleOnScreen());
            QCoreApplication::sendEvent(this, &event);
        }
        else
        { 
            FwHideEvent event;
            QCoreApplication::sendEvent(this, &event);
        }
    }
}

void FwWidget::apply(FwMLObject *object)
{
    BaseClass::apply(object);
    FwMLBool* darkBackground = object->attribute("darkBackground")->cast<FwMLBool>();
    if(darkBackground)
    {
        setDarkBackground(darkBackground);
    }
}

void FwWidget::setFont(const FwFont& font)
{
    if(m_font != font)
    {
        m_font = font;
        QEvent event(QEvent::FontChange);
        QCoreApplication::sendEvent(this, &event);
    }
}

void FwWidget::setShowAnimation(QPropertyAnimation* animation)
{
    if(m_showAnimation != animation)
    {
        if(m_showAnimation)
        {
            m_showAnimation->stop();
            m_showAnimation->disconnect(this);
            this->disconnect(m_showAnimation);
            m_showAnimation->deleteLater();
            m_showAnimation = 0;
        }
        if(animation)
        {
            m_showAnimation = animation;
            m_showAnimation->setParent(this);
            m_showAnimation->setObjectName("showAnimation");
            connect(m_showAnimation, SIGNAL(finished()), this, SLOT(showAnimationFinished()));
        }
    }
}

void FwWidget::setHideAnimation(QPropertyAnimation* animation)
{
    if(m_hideAnimation != animation)
    {
        if(m_hideAnimation)
        {
            m_hideAnimation->stop();
            m_hideAnimation->disconnect(this);
            this->disconnect(m_hideAnimation);
            m_hideAnimation->deleteLater();
            m_hideAnimation = 0;
        }
        if(animation)
        {
            m_hideAnimation = animation;
            m_hideAnimation->setParent(this);
            m_hideAnimation->setObjectName("hideAnimation");
            connect(m_hideAnimation, SIGNAL(finished()), this, SLOT(hideAnimationFinishel()));
        }
    }
}

void FwWidget::geometryChangedEvent(const QRect &oldRect, QRect &rect)
{
    if(oldRect.size() != rect.size())
    {
        FwResizeEvent event(m_oldSize, rect.size());
        QCoreApplication::sendEvent(this, &event);
    }
    FwRectPrimitive::geometryChangedEvent(oldRect, rect);
    m_childrenRect = rect;
}

void FwWidget::keyPressEvent(FwKeyPressEvent* event)
{
    Q_UNUSED(event);
}

void FwWidget::resizeEvent(FwResizeEvent* event)
{
    Q_UNUSED(event);
}

void FwWidget::requestAcceptEvent(FwResult* result)
{
    Q_UNUSED(result);
}

void FwWidget::setVisibleTime(int msec)
{
    if(m_visibleTime != msec)
    {
        (m_visibleTime = msec) ? startVisibleTimer() : stopVisibleTimer(); //TODO: Refactoring function startVisibleTimer!
    }
}

bool FwWidget::setVisible(bool visible)
{
    if(!BaseClass::setVisible(visible))
    {
        startVisibleTimer();
        return false;
    }
    return true;
}
