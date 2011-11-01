#ifndef FIREWORKS_DBFACTORY_HPP
#define FIREWORKS_DBFACTORY_HPP

#include "fwdb/fwdb.hpp"

namespace Fw
{
    FIREWORKSSHARED_EXPORT Database* dbFactory(FwMLObject* object, QObject* parent = 0) throw(Exception&);
    FIREWORKSSHARED_EXPORT Database* dbFactory(const QString& configFile, QObject* parent = 0) throw(Fw::Exception&);
}

#endif //FIREWORKS_DBFACTORY_HPP
