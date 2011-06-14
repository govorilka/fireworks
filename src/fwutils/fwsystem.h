#ifndef FIREWORKS_SYSTEM_H
#define FIREWORKS_SYSTEM_H

#include <QtCore/qstring.h>

namespace FwSystem
{
    bool exec(const QString& command, QString* output = 0);
}

#endif // FIREWORKS_SYSTEM_H
