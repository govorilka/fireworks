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

        //Fw::Database::QueryPtr query1 = db.createQuery("SELECT * FROMinsert into channels (\"id_xmltv\", \"ch_index\", \"id_category\", \"protocol\", \"host\", \"port\", \"audiopid\", \"caption\") values ('1001', '1', '1',   'udp', '239.211.211.100',  '1', '1234', 'Первый канал2')");
        //query1->step();

        Fw::Database::QueryPtr q = db.createQuery("insert into users "
                                                   " (login, password, birthday, lastlogon, age)"
                                                   " values (?1, ?2, ?3, ?4, ?5);");
        q->bindText(1,"user_1");
        q->bindText(2,"new_1234578920");
        q->bindDate(3, QDate(1990, 5, 10));
        q->bindDateTime(4, QDateTime(QDate(2008, 10, 5), QTime(12, 24, 30)));
        q->bindInt(5, 12);
        q->step();

        q->bindText(1,"user_2");
        q->bindText(2,"psw_78906557");
        q->bindDate(3, QDate(1975, 6, 6));
        q->bindDateTime(4, QDateTime(QDate(2010, 11, 2), QTime(18, 05, 22)));
        q->bindInt(5, 18);
        q->step();

        Fw::Database::QueryPtr selectQuery = db.createQuery("SELECT * FROM users");
        QTextStream qout(stdout, QIODevice::WriteOnly);
        while(selectQuery->step())
        {
            int id = selectQuery->columnInt(0);
            QString user_login = selectQuery->columnText(1);
            QString passwd = selectQuery->columnText(2);
            int age = selectQuery->columnInt(3);
            QDate birthday = selectQuery->columnDate(4);
            QDateTime last_login = selectQuery->columnDateTime(5);
            qout << "user_id: "     << id
                 << " login: "      << user_login
                 << " passwd: "     << passwd
                 << " age: "        << age
                 << " birthday: "   << birthday.toString(Qt::ISODate)
                 << " last login: " << last_login.toString(Qt::ISODate)
                 << endl;
            qout.flush();
        }

        db->commit();
        db->close();
    }
    catch(Fw::Exception& e)
    {
        qDebug() << "Exception occured:" << e.what();
    }

    return 0;
}
