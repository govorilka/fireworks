#include <QtCore/qdebug.h>
#include <QtCore/qfile.h>
#include <QtCore/qtranslator.h>

#include <QtGui/qapplication.h>
#include <QtGui/qstyle.h>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(QFile::exists("reactor_ru.qm"))
    {
        QTranslator* translator = new QTranslator();
        if(translator->load("reactor_ru.qm"))
        {
            QCoreApplication::installTranslator(translator);
        }
        else
        {
            delete translator;
        }
    }

    MainWindow w;
    w.show();

    return a.exec();
}
