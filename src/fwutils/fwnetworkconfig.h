#ifndef FIREWORKS_NETWORKCONFIG_H
#define FIREWORKS_NETWORKCONFIG_H

#include <QtCore/qobject.h>

#include <QtNetwork/qnetworkinterface.h>

#include "fireworks.h"

class FwMLObject;

class FIREWORKSSHARED_EXPORT FwNetworkConfig : public QObject
{
    Q_OBJECT
    typedef QObject BaseClass;

public:
    explicit FwNetworkConfig(QObject *parent = 0);

    inline QString activeInterfaceName() const;
    inline void setActiveInterfaceName(const QString& name);

    inline QNetworkInterface activeInterface() const;

    QHostAddress ip() const;
    QHostAddress netmask() const;
    QHostAddress firstDNS() const;
    QHostAddress secondDNS() const;
    QHostAddress gateway() const;

    QString mac(const QNetworkInterface& interface);

    void apply(FwMLObject* object);

private:
    QString m_activeInterface;
};

QString FwNetworkConfig::activeInterfaceName() const
{
    return m_activeInterface;
}

void FwNetworkConfig::setActiveInterfaceName(const QString& name)
{
    m_activeInterface = name;
}

QNetworkInterface FwNetworkConfig::activeInterface() const
{
    if(!m_activeInterface.isEmpty())
    {
        return QNetworkInterface::interfaceFromName(m_activeInterface);
    }
    return QNetworkInterface();
}

#endif //FIREWORKS_NETWORKCONFIG_H
