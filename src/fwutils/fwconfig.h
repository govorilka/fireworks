#ifndef FWCONFIG_H
#define FWCONFIG_H

#include <QtCore/qstring.h>

#include "fireworks.h"

#include "fw/exception.hpp"

class FwCPPObject;

class FIREWORKSSHARED_EXPORT FwConfig
{
public:
    FwConfig();

    static QString configPath();
    static QString configFilePath(const QString& fileName);

    static void loadConfig(const QString& name, FwCPPObject* object) throw (Fw::Exception&);
};

#endif // FWCONFIG_H
