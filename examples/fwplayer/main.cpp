#include <QtCore/qdebug.h>
#include <QtCore/qdir.h>
#include <QtCore/qfile.h>

#include <QtGui/QApplication>

#include "fwcore/fwml.h"

#include "fwgui/fwgraphicsview.h"

#include "playerscene.h"

#include "qpgraphicsview.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPGraphicsView view;

    PlayerScene* playerScene = new PlayerScene(1, &view);
    view.setActiveScene(playerScene);

    QWidget* widget = view.createWidget(0);
    widget->show();

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
