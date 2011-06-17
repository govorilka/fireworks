#ifndef FIREWORKS_NETWORKCONFIG_H
#define FIREWORKS_NETWORKCONFIG_H

#include <QtCore/qobject.h>

#include <QtNetwork/qnetworkinterface.h>

#include "fireworks.h"

#include "fwcore/fwcppobject.h"

class FIREWORKSSHARED_EXPORT FwNetworkConfig : public QObject, public FwCPPObject
{
    Q_OBJECT
    typedef FwCPPObject BaseClass;

public:
    explicit FwNetworkConfig(const QByteArray& name, QObject *parent = 0);

    inline QString activeInterfaceName() const;
    bool setActiveInterfaceName(const QString& name);

    QNetworkInterface activeInterface() const;

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

#endif //FIREWORKS_NETWORKCONFIG_H
