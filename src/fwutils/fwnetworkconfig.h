#ifndef FIREWORKS_NETWORKCONFIG_H
#define FIREWORKS_NETWORKCONFIG_H

#include <QtCore/qobject.h>

#include <QtNetwork/qnetworkinterface.h>

class FwMLObject;

class FwNetworkConfig : public QObject
{
    Q_OBJECT
    typedef QObject BaseClass;

public:
    explicit FwNetworkConfig(QObject *parent = 0);

    inline QString activeInterfaceName() const;
    inline void setActiveInterfaceName(const QString& name);

    inline QNetworkInterface activeInterface() const;

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

#endif // FIREWORKS_NETWORKCONFIG_H
