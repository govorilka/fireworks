#include <QtCore/qcoreapplication.h>
#include <QtCore/qdebug.h>

#include "fwdb/dbfactory.h"

extern void ConnectPG(const char*const query);
int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv); 

    QString connection;
    if(argc > 1)
    {
        connection = argv[1];
    }

    try
    {
        Fw::Database* db = dbFactory(0, "postgre", connection);
        db->beginTransaction();

//        Fw::Query query1 = db.query("DECLARE myportal CURSOR FOR select * from pg_database");
//        query1.step();

//        Fw::Query query2 = db.query("FETCH ALL in myportal");
//        while(query2.step())
//        {
//            printf("%-15s", query2.columnText(0).toUtf8().constData());
//        }

//        Fw::Query query3 = db.query("CLOSE myportal");
//        query3.step();

        Fw::Query query2 = db->query("SELECT contoencoding FROM pg_conversion");
        qDebug() << "contoencoding";
        while(query2.step())
        {
            qDebug() << query2.columnInt(0);
        }

        Fw::Query query3 = db->query("SELECT datallowconn FROM pg_database");
        qDebug() << "\n////////////////////////////////////////////\n";
        qDebug() << "datallowcon";
        while(query3.step())
        {
            if(query3.columnBool(0))
                qDebug() << "_true";
            else
                qDebug() << "_false";
        }

        db->commit();
        db->close();

        delete db;
        db = 0;
    }
    catch(Fw::Exception& e)
    {
        qDebug() << "Exception occured:" << e.what();
    }

    return 0;
}
