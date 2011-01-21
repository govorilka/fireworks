#include <QtCore/qdebug.h>

#include <QtGui/QApplication>

#include "fwgui/fwbackendloader.h"
#include "fwgui/fwgraphicsview.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    FwBackendLoader backendLoader;

    return a.exec();
}
