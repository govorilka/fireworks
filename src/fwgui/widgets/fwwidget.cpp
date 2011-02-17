#include <QtCore/qpropertyanimation.h>
#include <QtCore/qdebug.h>

#include <QtGui/qevent.h>
#include <QtGui/qapplication.h>

#include "fwwidget.h"

#include "fwgui/fwscene.h"

/*!
\class FwWidget
Класс реализует различные элементы пользовательского интерфейса.
*/
FwWidget::FwWidget(const QByteArray& name, FwPrimitiveGroup* parent) :
    BaseClass(name, parent),
    m_visibleTime(0),
    m_visibleTimerId(0)
{
    m_scene->m_widgets.append(this);
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
    switch(e->type())
    {
    case QEvent::FontChange:
        e->accept();
        fontChangedEvent(font());
        return true;

    default:
        break;
    }

    return BaseClass::event(e);
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

void FwWidget::keyPressEvent(QKeyEvent* event)
{
    Q_UNUSED(event);
}

void FwWidget::timerEvent(QTimerEvent* event)
{
    if(m_visibleTimerId == event->timerId())
    {
        hide();
    }
}

void FwWidget::showEvent(QShowEvent* event)
{
    event->accept();
}

void FwWidget::hideEvent(QHideEvent* event)
{
    event->accept();
}

void FwWidget::resizeEvent(QResizeEvent* event)
{
    event->accept();
}

void FwWidget::fontChangedEvent(const FwFont& font)
{
    Q_UNUSED(font);
}

void FwWidget::showAnimationFinished()
{
    startVisibleTimer();
    invalidate();
}

void FwWidget::hideAnimationFinished()
{
}

void FwWidget::visibleChangedEvent()
{
    BaseClass::visibleChangedEvent();
    if(isVisibleOnScreen())
    {
        QShowEvent event;
        showEvent(&event);
        showAnimationFinished();
    }
    else
    {
        QHideEvent event;
        hideEvent(&event);
        stopVisibleTimer();
    }
}