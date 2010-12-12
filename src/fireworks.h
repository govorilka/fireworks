#ifndef FIREWORKS_H
#define FIREWORKS_H

#include <QtCore/qglobal.h>

#if defined(FIREWORKS_LIBRARY)
#  define FIREWORKSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FIREWORKSSHARED_EXPORT Q_DECL_IMPORT
#endif

class FIREWORKSSHARED_EXPORT Fireworks {
public:
    Fireworks();
};

#endif // FIREWORKS_H
