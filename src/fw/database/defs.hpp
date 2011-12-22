#ifndef FIREWORKS_DATABASE_DEFS_HPP
#define FIREWORKS_DATABASE_DEFS_HPP

#include <QtCore/qsharedpointer.h>

#include "fireworks.h"


class QObject;
class FwMLObject;

namespace Fw
{
    namespace Database
    {
    class Controller;

    class Driver;
    class Query;
    class Transaction;

    typedef QSharedPointer<Driver> DriverPtr;
    typedef QSharedPointer<Query> QueryPtr;
    }
}

#endif // FIREWORKS_DATABASE_DEFS_HPP
