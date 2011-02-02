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
    m_autoRepeat(false)
{
}

FwKeyPressEvent::FwKeyPressEvent(int key, bool autoRepeat) :
    BaseClass(),
    m_key(key),
    m_autoRepeat(autoRepeat)
{
}
