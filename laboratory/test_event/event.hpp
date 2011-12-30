#ifndef EVENT_HPP
#define EVENT_HPP

#include <QtCore/qcoreevent.h>

template<class T>
class Event : public QEvent
{
    typedef QEvent BaseClass;

public:

    static const int s_typeID;

    Event() :
        BaseClass(static_cast<QEvent::Type>(s_typeID))
    {
    }

    T data;
};

template<class T>
const int Event<T>::s_typeID = QEvent::registerEventType();

#endif // EVENT_HPP
