#include <QtCore/qdebug.h>
#include <QtCore/qdir.h>
#include <QtCore/qfile.h>

#include <QtGui/QApplication>

#include "fwcore/fwml.h"

#include "fwgui/fwgraphicsview.h"

#include "playerscene.h"
#include "playerscene2.h"
#include "playerscene3.h"
#include "playerscene4.h"
#include "playerscene5.h"
#include "playerscene6.h"

#include "qpgraphicsview.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPGraphicsView view;

    PlayerScene* playerScene = new PlayerScene(1, &view);
    view.setActiveScene(playerScene);

    QFile fwmlFile(a.applicationDirPath() + QDir::separator() + "example.fwml");
    if(fwmlFile.exists())
    {
        FwMLObject rootObject;
        if(rootObject.parse(&fwmlFile))
        {
            playerScene->apply(&rootObject);
        }
    }

    PlayerScene2* playerScene2 = new PlayerScene2(2, &view);
    QFile fwmlFile2(a.applicationDirPath() + QDir::separator() + "example2.fwml");
    if(fwmlFile2.exists())
    {
        FwMLObject rootObject;
        if(rootObject.parse(&fwmlFile2))
        {
            playerScene2->apply(&rootObject);
        }
    }

    PlayerScene3* playerScene3 = new PlayerScene3(3, &view);
    QFile fwmlFile3(a.applicationDirPath() + QDir::separator() + "example3.fwml");
    if(fwmlFile3.exists())
    {
        FwMLObject rootObject;
        if(rootObject.parse(&fwmlFile3))
        {
            playerScene3->apply(&rootObject);
        }
    }

    PlayerScene4* playerScene4 = new PlayerScene4(4, &view);
    QFile fwmlFile4(a.applicationDirPath() + QDir::separator() + "example4.fwml");
    if(fwmlFile4.exists())
    {
        FwMLObject rootObject;
        if(rootObject.parse(&fwmlFile4))
        {
            playerScene4->apply(&rootObject);
        }
    }

    PlayerScene5* playerScene5 = new PlayerScene5(5, &view);
    QFile fwmlFile5(a.applicationDirPath() + QDir::separator() + "example5.fwml");
    if(fwmlFile5.exists())
    {
        FwMLObject rootObject;
        if(rootObject.parse(&fwmlFile5))
        {
            playerScene5->apply(&rootObject);
        }
    }

    PlayerScene6* playerScene6 = new PlayerScene6(6, &view);
    QFile fwmlFile6(a.applicationDirPath() + QDir::separator() + "example6.fwml");
    if(fwmlFile6.exists())
    {
        FwMLObject rootObject;
        if(rootObject.parse(&fwmlFile6))
        {
            playerScene6->apply(&rootObject);
        }
    }

    QWidget* widget = view.createWidget(0);
    widget->show();

    return a.exec();
}
