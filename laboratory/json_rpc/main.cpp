
#include <QtCore/QCoreApplication>
#include <QtCore/qdebug.h>

#include "jsonrpc.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    //////////////////////

    JsonRpc jsonRpc(&app);

    return app.exec();
}

