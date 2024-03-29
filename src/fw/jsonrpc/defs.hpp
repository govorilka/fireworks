#ifndef FIREWORKS_JSONRPC_DEFS_HPP
#define FIREWORKS_JSONRPC_DEFS_HPP

#include <QtCore/qglobal.h>

#if defined(FW_JSONRPC_LIBRARY)
#  define FW_JSONRPC_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define FW_JSONRPC_SHARED_EXPORT Q_DECL_IMPORT
#endif

namespace Fw
{
    namespace JSON
    {
        class RPC;
    }
}

#endif //FIREWORKS_JSONRPC_DEFS_HPP
