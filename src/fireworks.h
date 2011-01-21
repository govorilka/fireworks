#ifndef FIREWORKS_H
#define FIREWORKS_H

#include <QtCore/qglobal.h>

#if defined(FIREWORKS_LIBRARY)
#  define FIREWORKSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FIREWORKSSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace Fw
{
    enum Position
    {
        P_Relative,
        P_Absolute,
        P_Fixed,
    };

    enum BufferMode
    {
        BM_NoBuffer = 0,
        BM_SystemDefault,
        BM_VideoOnly,
        BM_SystemOnly,
    };

    enum Orientation
    {
        O_Horizontal,
        O_Vertical,
        O_Diagonal
    };
}

class FIREWORKSSHARED_EXPORT Fireworks
{
public:
    Fireworks();
};

#endif // FIREWORKS_H
