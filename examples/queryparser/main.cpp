#include <QtCore/qcoreapplication.h>
#include <QtCore/qdebug.h>

#include "fwdb/fwpg.h"

extern void ConnectPG(const char*const query);
int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv); 

    qDebug() << "#Parse begin" << endl;
    QString query("select * from  ?? where id = ?1 and name=?2");

    FwPg::TokenVector tokens;
    FwPg::parseQuery(query.toUtf8(), tokens);

    qDebug() << query << endl;
    foreach(FwPg::QueryToken token, tokens)
    {
        qDebug() << token.param << token.value;
    }

    qDebug() << endl << "#Parse end" << endl;

    QString connection;
    if(argc > 1)
    {
        connection = argv[1];
    }

    //ConnectPG(connection.toUtf8().constData());

    FwPg::Database pgdb;
    Fw::Database& db = pgdb;

    try
    {
        db.open(connection);
        db.beginTransaction();

        Fw::Query query1 = db.query("DECLARE myportal CURSOR FOR select * from pg_database");
        query1.step();

        Fw::Query query2 = db.query("FETCH ALL in myportal");
        while(query2.step())
        {
            printf("%-15s", query2.columnText(0).toUtf8().constData());
        }

        Fw::Query query3 = db.query("CLOSE myportal");
        query3.step();

        db.commit();
        db.close();
    }
    catch(Fw::Exception& e)
    {
        qDebug() << "Exception occured:" << e.what();
    }

    return 0;
}
