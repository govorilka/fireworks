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
        template<typename Interface> class Plugin;
        class Controller;

        class Driver;
        class DriverLoader;

        class Query;
        class Transaction;

        typedef QSharedPointer<DriverLoader> DriverLoaderPtr;
        typedef QSharedPointer<Query> QueryPtr;

        class PluginInterface;
        class PluginLoder;
    }
}

#endif // FIREWORKS_DATABASE_DEFS_HPP
