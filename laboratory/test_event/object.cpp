#include <QtCore/QEvent>
#include <QtCore/qdebug.h>

#include "object.hpp"

Object::Object(QObject* parent) :
    BaseClass(parent)
{
}

bool Object::event(QEvent* e)
{
    if(e->type() == QEvent::User)
    {
        qDebug() << "User event";
    }
    return BaseClass::event(e);
}
