#include <QtCore/QCoreApplication>
#include <QtCore/qdebug.h>

#include <fwdb/dbfactory.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "Main";
    try
    {
        Fw::Database* db = Fw::dbFactory("postgres_test.fwml", 0);
        qDebug() << "DB is open!";


        Fw::Query q = db->query("insert into users \
                                          (login, password, birthday, lastlogon, age)\
                                          values (?1, ?2, ?3, ?4, ?5);");
            q.bindText(1,"user_1");
            q.bindText(2,"psw 12345");
            q.bindDate(3, QDate(1990, 5, 10));
            q.bindDateTime(4, QDateTime(QDate(2008, 10, 5), QTime(12, 24, 30)));
            q.bindInt(5, 12);
            q.step();

            q.bindText(1,"user_2");
            q.bindText(2,"psw 78906557");
            q.bindDate(3, QDate(1975, 6, 6));
            q.bindDateTime(4, QDateTime(QDate(2010, 11, 2), QTime(18, 05, 22)));
            q.bindInt(5, 18);
            q.step();

            Fw::Query selectQuery = db->query("SELECT * FROM users");
            QTextStream qout(stdout, QIODevice::WriteOnly);

            while(selectQuery.step())
            {
                int id = selectQuery.columnInt(0);
                QString user_login = selectQuery.columnText(1);
                QString passwd = selectQuery.columnText(2);
                int age = selectQuery.columnInt(3);
                QDate birthday = selectQuery.columnDate(4);
                QDateTime last_login = selectQuery.columnDateTime(5);
                qout << "user_id: "     << id
                     << " login: "      << user_login
                     << " passwd: "     << passwd
                     << " age: "        << age
                     << " birthday: "   << birthday.toString(Qt::ISODate)
                     << " last login: " << last_login.toString(Qt::ISODate)
                     << endl;
                qout.flush();
            }

        delete db;
        db = 0;
    }
    catch(Fw::Exception& e)
    {
        qDebug() << "exception:" << e.what();
    }

    return a.exec();
}
