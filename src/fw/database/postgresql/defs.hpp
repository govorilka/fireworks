#ifndef FIREWORKS_DATABASE_POSTGREQL_DEFS_HPP
#define FIREWORKS_DATABASE_POSTGREQL_DEFS_HPP

#include "fw/database/defs.hpp"

#ifdef FW_SUPPORT_POSTGRESQL

#ifdef Q_OS_WIN32
#include <libpq-fe.h>
#else //Q_OS_WIN32
#include "pgsql/libpq-fe.h"
#endif //Q_OS_WIN32

namespace Fw
{
    namespace Database
    {
        namespace PostgreSQL
        {
            class Driver;
            class Query;
        }
    }
}

#endif //FW_SUPPORT_POSTGRESQL

#endif // FIREWORKS_DATABASE_POSTGREQL_DEFS_HPP
