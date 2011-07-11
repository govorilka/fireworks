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

int FwKeyPressEvent::digit() const
{
    switch(m_key)
    {
    case Qt::Key_0:
        return 0;

    case Qt::Key_1:
        return 1;

    case Qt::Key_2:
        return 2;

    case Qt::Key_3:
        return 3;

    case Qt::Key_4:
        return 4;

    case Qt::Key_5:
        return 5;

    case Qt::Key_6:
        return 6;

    case Qt::Key_7:
        return 7;

    case Qt::Key_8:
        return 8;

    case Qt::Key_9:
        return 9;

    default:
        break;
    }

    return -1;
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
