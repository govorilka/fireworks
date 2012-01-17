#ifndef FIREWORKS_DATABASE_POSTGREQL_DEFS_HPP
#define FIREWORKS_DATABASE_POSTGREQL_DEFS_HPP

#include "fw/database/defs.hpp"

#ifdef FW_SUPPORT_POSTGRESQL

//#define UBUNTU_11_10 //Please defines in Ubuntu 11.10 only

#if defined Q_OS_WIN32
    #include <libpq-fe.h>
#else
    #if defined UBUNTU_11_10
        #include "postgresql/libpq-fe.h"
    #else //Q_OS_WIN32
        #include "pgsql/libpq-fe.h"
    #endif
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
