#ifndef FIREWORKS_DBFACTORY_HPP
#define FIREWORKS_DBFACTORY_HPP

#include "fwdb/fwdb.h"

namespace Fw
{
    Database* dbFactory(QObject* parent, const QString& driver, FwMLObject* config) throw(Exception&);
    Database* dbFactory(QObject* parent, const QString& configFile) throw(Fw::Exception&);
}

#endif //FIREWORKS_DBFACTORY_HPP
