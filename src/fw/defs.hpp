#ifndef FIREWORKS_DEFS_HPP
#define FIREWORKS_DEFS_HPP

#include <QtCore/qglobal.h>

#if defined(FIREWORKS_LIBRARY)
#  define FIREWORKSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FIREWORKSSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace Fw
{
    class Exception;
}

#endif // FIREWORKS_DEFS_HPP
