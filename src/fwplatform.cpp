
#include "fwplatform.hpp"

Fw::Platform::Platform(QObject* parent) :
    QObject(parent),
    BaseClass("Fw::Platform")
{
}

Fw::Platform::~Platform()
{
}

bool Fw::Platform::loadData(FwMLObject* object)
{
    //TODO
    Q_UNUSED(object);
    return false;
}
