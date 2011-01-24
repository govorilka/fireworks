#include <QtCore/qdebug.h>
#include <QtCore/qdir.h>
#include <QtCore/qfile.h>

#include <QtGui/QApplication>

#include "fwcore/fwml.h"

#include "fwgui/fwbackendloader.h"
#include "fwgui/fwgraphicsview.h"

#include "playerscene.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    FwBackendLoader backendLoader;
    FwGraphicsView* view = backendLoader.loadView(a.applicationDirPath() + QDir::separator() + "libqp_backend.so");
    if(!view)
    {
        qDebug() << backendLoader.errorString();
        return -1;
    }

    PlayerScene* playerScene = new PlayerScene(1, view);
    view->setActiveScene(playerScene);

    QFile fwmlFile(a.applicationDirPath() + QDir::separator() + "example.fwml");
    if(fwmlFile.exists())
    {
        FwMLObject rootObject;
        if(rootObject.parse(&fwmlFile))
        {
            playerScene->apply(&rootObject);
        }
    }

    return a.exec();
}
