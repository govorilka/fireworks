#ifndef FIREWORKS_DATABASE_FACTORY_HPP
#define FIREWORKS_DATABASE_FACTORY_HPP

#include "fw/database/defs.hpp"

namespace Fw
{
    namespace Database
    {
        FIREWORKSSHARED_EXPORT Driver* factory(FwMLObject* object) throw(Exception&);
        FIREWORKSSHARED_EXPORT Driver* factory(const QString& configFile) throw(Fw::Exception&);
    }
}

#endif // FIREWORKS_DATABASE_FACTORY_HPP
