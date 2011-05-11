#ifndef FIREWORKS_NETWORKINFO_H
#define FIREWORKS_NETWORKINFO_H

#include <QtCore/qobject.h>

class FwNetworkInfo : public QObject
{
    Q_OBJECT
    typedef QObject BaseClass;

public:
    explicit FwNetworkInfo(QObject *parent = 0);


};

#endif //FIREWORKS_NETWORKINFO_H
