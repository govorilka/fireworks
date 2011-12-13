#include <QtCore/qdebug.h>
#include <QtCore/qdir.h>
#include <QtCore/qfile.h>

#include <QtGui/QApplication>

#include "fwcore/fwml.h"
#include "fwcore/fwmlengine.h"

#include "fwgui/fwgraphicsview.h"

#include "playerscene.h"
#include "playerscene2.h"
#include "playerscene3.h"
#include "playerscene4.h"
#include "playerscene5.h"
#include "playerscene6.h"

#include "qpgraphicsview.h"

bool loadScene(FwGraphicsView* view)
{
    new PlayerScene(view);
    new PlayerScene2(view);
    new PlayerScene3(view);
    new PlayerScene4(view);
    new PlayerScene5(view);
    new PlayerScene6(view);

    FwMLEngine engine;
    engine.setRootDirectory(QCoreApplication::applicationDirPath() + QDir::separator() + "examples");
    return view->loadData(&engine);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPGraphicsView view;
    if(!view.init())
    {
        qDebug() << "Cannot init view";
        return -1;
    }

    if(!loadScene(&view))
    {
        qDebug() << "Cannot load scenes";
        return -1;
    }

    QWidget* widget = view.createWidget(0);
    widget->show();

    return a.exec();
}
