#ifndef FWCONFIG_H
#define FWCONFIG_H

#include <QtCore/qstring.h>

class FwCPPObject;

class FwConfig
{
public:
    FwConfig();

    static QString configPath();
    static QString configFilePath(const QString& fileName);

    static bool loadConfig(const QString& name, FwCPPObject* object);
};

#endif // FWCONFIG_H
