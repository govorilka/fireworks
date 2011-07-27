#include "fwutils/fwsystem.h"

#include <QtCore/qprocess.h>

bool FwSystem::exec(const QString& command, QString* output)
{
    QProcess process;
    process.start(command);
    if(process.waitForStarted())
    {
        process.waitForFinished();

        if(output)
        {
            (*output) = process.readAll().simplified();
        }

        process.terminate();
        return true;
    }
    return false;
}
