#include "fwcppobject.h"

FwCPPObject::FwCPPObject(const QByteArray& name) :
    m_name(name)
{
}

void FwCPPObject::apply(FwMLObject *object)
{
    Q_UNUSED(object);
}
