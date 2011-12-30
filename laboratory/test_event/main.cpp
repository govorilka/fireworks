#include <QtCore/QCoreApplication>
#include <QtCore/qdebug.h>

#include "object.hpp"
#include "event.hpp"

class A
{
    int a;
};

class B
{
    int b;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Object* object = new Object(&a);

    Event<A>* event1 = new Event<A>();
    Event<A>* event2 = new Event<A>();
    Event<B>* event3 = new Event<B>();

    qDebug() << event1->type();
    qDebug() << event2->type();
    qDebug() << event3->type();

    a.postEvent(object, event1);

    return a.exec();
}
