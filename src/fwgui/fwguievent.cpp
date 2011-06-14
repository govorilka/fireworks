#include "fwguievent.h"

FwGuiEvent::FwGuiEvent() :
    BaseClass(static_cast<QEvent::Type>(qtTypeID()))
{
}

int FwGuiEvent::qtTypeID()
{
    static int _typeID = QEvent::registerEventType();
    return _typeID;
}

//////////////////////////////////////////////////////////////////////////////

FwKeyPressEvent::FwKeyPressEvent(int key, int autoRepeatCount) :
    BaseClass(),
    m_key(key),
    m_autoRepeatCount(autoRepeatCount)
{
}

//////////////////////////////////////////////////////////////////////////////

FwResizeEvent::FwResizeEvent(const QSize& olsSize, const QSize& newSize) :
    BaseClass()
{
}

//////////////////////////////////////////////////////////////////////////////

FwShowEvent::FwShowEvent(bool visibleOnScreen) :
    BaseClass(),
    m_visibleOnScreen(visibleOnScreen)
{
}

//////////////////////////////////////////////////////////////////////////////

FwHideEvent::FwHideEvent() :
    BaseClass()
{
}

//////////////////////////////////////////////////////////////////////////////

FwSceneShowEvent::FwSceneShowEvent(FwScene* previous) :
    BaseClass(),
    m_previous(previous)
{
}

//////////////////////////////////////////////////////////////////////////////

FwSceneHideEvent::FwSceneHideEvent(FwScene* next) :
    BaseClass(),
    m_next(next)
{
}
