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

///////////////////////////////////////////////////////////////////////////////

FwKey::FwKey() :
    m_qtKey(0),
    m_number(-1),
    m_ansciiChar(0)
{
}

FwKey::FwKey(quint32 qtKey, qint8 number, char ansciiChar) :
    m_qtKey(qtKey),
    m_number(number),
    m_ansciiChar(ansciiChar)
{
}

///////////////////////////////////////////////////////////////////////////////

FwKeyPressEvent::FwKeyPressEvent(const FwKey& key, int autoRepeatCount) :
    BaseClass(),
    m_key(key),
    m_autoRepeatCount(autoRepeatCount)
{
    if(key.ansciiChar())
    {
        m_text = QChar(key.ansciiChar());
    }
}

//////////////////////////////////////////////////////////////////////////////

FwResizeEvent::FwResizeEvent(const QSize& oldSize, const QSize& newSize) :
    BaseClass(),
    m_oldSize(oldSize),
    m_newSize(newSize)
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
