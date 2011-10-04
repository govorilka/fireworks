#include <QtGui/QApplication>

#include <QtGui/qwidget.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qcalendarwidget.h>

#include "controll.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Controll ctrl;

    ctrl.show();
    return a.exec();
}
