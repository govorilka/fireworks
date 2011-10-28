#ifndef FIREWORKS_DBFACTORY_HPP
#define FIREWORKS_DBFACTORY_HPP

#include "fwdb/fwdb.h"

namespace Fw
{
    FIREWORKSSHARED_EXPORT Database* dbFactory(QObject* parent, const QString& driver, FwMLObject* config, bool* createdDB = 0) throw(Exception&);
    FIREWORKSSHARED_EXPORT Database* dbFactory(QObject* parent, const QString& configFile, bool* createdDB = 0) throw(Fw::Exception&);
}

#endif //FIREWORKS_DBFACTORY_HPP
