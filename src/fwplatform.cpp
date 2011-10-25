
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
    return false;
}
