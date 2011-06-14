#include "fwcore/fwml.h"

#include "fwnetworkconfig.h"

FwNetworkConfig::FwNetworkConfig(const QByteArray& name, QObject *parent) :
    QObject(parent),
    BaseClass(name)
{
}

void FwNetworkConfig::apply(FwMLObject* object)
{
    FwMLString* interfaceNode = object->attribute("interface")->cast<FwMLString>();
    if(interfaceNode && !interfaceNode->value().isEmpty())
    {
        foreach(QNetworkInterface interface, QNetworkInterface::allInterfaces())
        {
            qDebug() << interface.name() << interface.humanReadableName();
        }

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

QString FwNetworkConfig::mac(const QNetworkInterface& interface)
{
    return QString("00:00:00:00:00:00");
}

QHostAddress FwNetworkConfig::firstDNS() const
{
    return QHostAddress();
}
QHostAddress FwNetworkConfig::gateway() const
{
    return QHostAddress();
}

