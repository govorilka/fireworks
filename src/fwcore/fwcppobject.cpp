#include <QtCore/qcoreapplication.h>

#include <fwcore/fwml.h>

#include "fwcppobject.h"

FwCPPObject::FwCPPObject(const QByteArray& name) :
    m_name(name)
{
}

void FwCPPObject::resetData()
{
}

void FwCPPObject::loadFile(const QString& fileName) throw(Fw::Exception&)
{
    FwMLObject rootObject;
    rootObject.parseFile(fileName);
    loadData(&rootObject);
}

