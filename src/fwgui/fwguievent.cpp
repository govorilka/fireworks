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

FwKeyPressEvent::FwKeyPressEvent() :
    BaseClass(),
    m_key(0),
    m_autoRepeatCount(0)
{
}

FwKeyPressEvent::FwKeyPressEvent(int key, int autoRepeatCount) :
    BaseClass(),
    m_key(key),
    m_autoRepeatCount(autoRepeatCount)
{
}
