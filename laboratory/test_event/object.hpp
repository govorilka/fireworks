#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <QtCore/qobject.h>

class Object : public QObject
{
    Q_OBJECT
    typedef QObject BaseClass;

public:
    Object(QObject* parent = 0);

    virtual bool event(QEvent* e);
};

#endif // OBJECT_HPP
