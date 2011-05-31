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

QHostAddress FwNetworkConfig::ip() const
{
    QList<QNetworkAddressEntry> addressList = activeInterface().addressEntries();
    return addressList.isEmpty() ? QHostAddress() : addressList.first().ip();
}

QHostAddress FwNetworkConfig::netmask() const
{
    QList<QNetworkAddressEntry> addressList = activeInterface().addressEntries();
    return addressList.isEmpty() ? QHostAddress() : addressList.first().netmask();
}

#if defined(Q_OS_LINUX)
    #include "fwnetworkconfig_linux.cpp"
#elif defined(Q_OS_WIN32)
    #include "fwnetworkconfig_win32.cpp"
#else
    #error This OS is not supported
#endif
