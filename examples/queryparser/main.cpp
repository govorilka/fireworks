#include <QtCore/qcoreapplication.h>
#include <QtCore/qdebug.h>

#include "fw/database/controller.hpp"
#include "fwcore/fwml.h"

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
        FwMLObject script;
        script.parseFile(connection);

        Fw::Database::Controller db;
        db.loadData(&script);
        db->open();

        db->beginTransaction();

        Fw::Database::QueryPtr query1 = db.createQuery("insert into channels (\"id_xmltv\", \"ch_index\", \"id_category\", \"protocol\", \"host\", \"port\", \"audiopid\", \"caption\") values ('1001', '1', '1',   'udp', '239.211.211.14',  '1', '1234', 'Первый канал2')");
        query1->step();
//        qDebug() << "contoencoding";
//        while(query1->step())
//        {
//            qDebug() << query1->columnInt(0);
//        }

//        Fw::Database::QueryPtr query1 = db.createQuery("SELECT datallowconn FROM pg_database");
//        qDebug() << "\n////////////////////////////////////////////\n";
//        qDebug() << "datallowcon";
//        while(query3->step())
//        {
//            if(query3->columnBool(0))
//                qDebug() << "_true";
//            else
//                qDebug() << "_false";
//        }

        db->commit();
        db->close();
    }
    catch(Fw::Exception& e)
    {
        qDebug() << "Exception occured:" << e.what();
    }

    return 0;
}
