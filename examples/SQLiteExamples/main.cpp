#include <QtCore/QCoreApplication>
#include <QtCore/qdebug.h>

#include "fwdb/fwdb.h"

#include "dbwork.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    try
    {
       DbWork db;
#ifdef USE_SQLITE_API
       db.loadDB(QString(a.applicationDirPath() + "/test"));
#else
       db.loadDB(QString(a.applicationDirPath() + "/configure.fwml"));
#endif
       db.selectQuery();
    }
    catch(Fw::Exception& e)
    {
        qDebug() << e.what();
    }

    return 0;
}
