#include <QtCore/QCoreApplication>
#include <QtCore/qdebug.h>

#include <fwdb/fwpg.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Fw::Database* db = new FwPg::Database();

    db->open();

    delete db;
    db = 0;
    return a.exec();
}
