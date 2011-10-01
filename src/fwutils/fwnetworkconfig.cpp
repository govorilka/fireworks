#include "fwcore/fwml.h"

#include "fwnetworkconfig.h"
#include "fwsystem.h"

#ifdef Q_OS_LINUX

#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/if.h>

#endif

FwNetworkConfig::FwNetworkConfig(const QByteArray& name, QObject *parent) :
    QObject(parent),
    BaseClass(name)
{
}

bool FwNetworkConfig::loadData(FwMLObject* object)
{
    FwMLString* interfaceNode = object->attribute("interface")->cast<FwMLString>();
    if(interfaceNode && !interfaceNode->value().isEmpty())
    {
        setActiveInterfaceName(QString::fromUtf8(interfaceNode->value()));
    }
    return true;
}

bool FwNetworkConfig::setActiveInterfaceName(const QString& name)
{
    if(m_activeInterface != name)
    {
        QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
        if(!interfaces.isEmpty())
        {
            foreach(QNetworkInterface interface, interfaces)
            {
                if(interface.name() == name)
                {
                    m_activeInterface = name;
                    return true;
                }
            }
        }
        return false;
    }

    return true;
}

QNetworkInterface FwNetworkConfig::activeInterface() const
{
    if(m_activeInterface.isEmpty())
    {
        QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
        if(!interfaces.isEmpty())
        {
            foreach(QNetworkInterface interface, interfaces)
            {
                if(!interface.flags().testFlag(QNetworkInterface::IsLoopBack))
                {
                    return interface;
                }
            }
            return interfaces.first();
        }
        return QNetworkInterface();
    }

    return QNetworkInterface::interfaceFromName(m_activeInterface);
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

#ifdef Q_OS_LINUX

QHostAddress FwNetworkConfig::firstDNS() const
{
    QString output;
    if(FwSystem::exec("cat /etc/resolv.conf", &output) && !output.isEmpty())
    {
        output = output.right(output.size() - output.indexOf("nameserver ") - 11);
        int spaceIndex = output.indexOf(' ');
        if(spaceIndex != -1)
        {
            output = output.left(spaceIndex);
        }
        return QHostAddress(output);
    }
    return QHostAddress();
}

QHostAddress FwNetworkConfig::secondDNS() const
{
    return QHostAddress();
}

QString FwNetworkConfig::mac(const QNetworkInterface& interface)
{
    if(interface.isValid())
    {
        int s = socket(AF_INET, SOCK_DGRAM, 0);
        if (s != -1)
        {
            char buf[1024];

            ifconf ifc;
            ifc.ifc_len = sizeof(buf);
            ifc.ifc_buf = buf;

            ioctl(s, SIOCGIFCONF, &ifc);

            ifreq* ifrReq = ifc.ifc_req;
            for(int i = ifc.ifc_len / sizeof(ifreq); --i >= 0; ifrReq++)
            {
                if(interface.name() == ifrReq->ifr_name)
                {
                    ifreq ifr;
                    strcpy(ifr.ifr_name, ifrReq->ifr_name);
                    if(ioctl(s, SIOCGIFFLAGS, &ifr) == 0
                        && !(ifr.ifr_flags & IFF_LOOPBACK)
                        && ioctl(s, SIOCGIFHWADDR, &ifr) == 0)
                    {
                        u_char addr[6];
                        bcopy( ifr.ifr_hwaddr.sa_data, addr, 6);
                        close(s);
                        return QString("%1:%2:%3:%4:%5:%6")
                            .arg(static_cast<uint>(addr[0]), 2, 16, QChar('0'))
                            .arg(static_cast<uint>(addr[1]), 2, 16, QChar('0'))
                            .arg(static_cast<uint>(addr[2]), 2, 16, QChar('0'))
                            .arg(static_cast<uint>(addr[3]), 2, 16, QChar('0'))
                            .arg(static_cast<uint>(addr[4]), 2, 16, QChar('0'))
                            .arg(static_cast<uint>(addr[5]), 2, 16, QChar('0'))
                            .toUpper();
                    }
                }
            }
            close(s);
        }
    }
    return QString();
}

QHostAddress FwNetworkConfig::gateway() const
{
    QString output;
    if(FwSystem::exec("iproute", &output) && !output.isEmpty())
    {
        output = output.right(output.size() - output.indexOf("default via ") - 12);
        return QHostAddress(output.left(output.indexOf(' ')));
    }
    return QHostAddress();
}

#else

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

#endif




