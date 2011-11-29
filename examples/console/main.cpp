#include <QtCore/qcoreapplication.h>
#include <QtCore/qdebug.h>

#include "fw/console/arguments.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Fw::Console::Arguments arguments(a.arguments());

    Fw::Console::ArgumentList argList = arguments.arguments();

    foreach(Fw::Console::Argument argument, argList)
    {
        qDebug() << QString("%1 %2").arg(argument.name).arg(argument.value);
    }

    return 0;
}
