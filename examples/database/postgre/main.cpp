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
        qDebug() << "DB is open";

        if(argc >= 2)
        {
            QFile file(argv[1]);
            if(file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QString str = QString::fromUtf8(file.readAll());
                Fw::Query q = db->query(str);

                qDebug() << str;
                q.bindInt(1, 88);
                q.bindText(2, "user4");

                while(q.step())
                {
                    qDebug() << q.columnInt(0) << q.columnText(1) << q.columnText(2) << q.columnInt(3)\
                             << q.columnInt(4) << q.columnText(5) << q.columnText(6);
                }
            }
        }

        delete db;
        db = 0;
    }
    catch(Fw::Exception& e)
    {
        qDebug() << "exception:" << e.error();
    }

    return a.exec();
}
