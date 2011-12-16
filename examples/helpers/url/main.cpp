
#include <QtCore/QCoreApplication>
#include <Qt/qdebug.h>

#include "fw/helpers/url.hpp"

using namespace Fw::Helpers;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    qDebug() << Url::Path(QUrl("http://tv.211.ru/tvguide/days.index"));

    return app.exec();
}
