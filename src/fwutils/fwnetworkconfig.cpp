#include "fwcore/fwml.h"

#include "fwnetworkconfig.h"

FwNetworkConfig::FwNetworkConfig(QObject *parent) :
    BaseClass(parent)
{
}

void FwNetworkConfig::apply(FwMLObject* object)
{
    FwMLString* interfaceNode = object->attribute("interface")->cast<FwMLString>();
    if(interfaceNode && !interfaceNode->value().isEmpty())
    {
        setActiveInterfaceName(QString::fromUtf8(interfaceNode->value()));
    }
}
