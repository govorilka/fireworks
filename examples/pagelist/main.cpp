#include "menuscene.h"

#include <QtCore/qdebug.h>
#include <QtCore/qdir.h>
#include <QtCore/qfile.h>

#include <QtGui/QApplication>

#include "fwcore/fwml.h"

#include "fwgui/fwgraphicsview.h"

#include "menuscene.h"

#include "qpgraphicsview.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QPGraphicsView view;

    MenuScene* menuScene = new MenuScene(1, &view);
    view.setActiveScene(menuScene);

    QWidget* widget = view.createWidget(0);
    widget->show();

    QFile fwmlFile(app.applicationDirPath() + QDir::separator() + "pagelist.fwml");
    if(fwmlFile.exists())
    {
        FwMLObject rootObject;
        if(rootObject.parse(&fwmlFile))
        {
            menuScene->apply(&rootObject);
        }
    }

    return app.exec();
}