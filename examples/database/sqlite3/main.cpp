#include <QtCore/qcoreapplication.h>
#include <QtCore/qtextstream.h>

#include "fwcore/fwmldocument.h"
#include "fw/database/controller.hpp"

int main(int argc, char **argv)
{
    QCoreApplication a(argc, argv);

    try
    {
        Fw::Database::Controller db;
        FwMLObject rootObject;
        rootObject.parseFile(":/sqlite3/local.fwml");
        db.loadData(&rootObject);
        db->open();

        Fw::Database::QueryPtr insertQuery = db.createQuery("INSERT INTO "
                                                "tempTable ( int_value, real_value, text_value ) "
                                                "VALUES ( ?1, ?2, ?3 );"
                                                );

        insertQuery->bindInt(1, 123);
        insertQuery->bindDouble(2, 123.456);
        insertQuery->bindText(3, "123456");
        insertQuery->step();

        insertQuery->bindInt(1, 456);
        insertQuery->bindDouble(2, 456.789);
        insertQuery->bindText(3, "456789");
        insertQuery->step();

        insertQuery->bindInt(1, 789);
        insertQuery->bindDouble(2, 789.0);
        insertQuery->bindText(3, "7890");
        insertQuery->step();

        Fw::Database::QueryPtr selectQuery = db.createQuery("SELECT "
                                                            "id, int_value, real_value, text_value "
                                                            "FROM tempTable;"
                                                );

        while(selectQuery->step())
        {
            qDebug() << "id: " << selectQuery->columnInt(0);
            qDebug() << "int_value: " << selectQuery->columnInt(1);
            qDebug() << "real_value: " << selectQuery->columnDouble(2);
            qDebug() << "text_value: " << selectQuery->columnText(3);
        }

    }
    catch(const Fw::Exception& e)
    {
        qDebug() << "Exception:" << e.error();
        return -1;
    }
    catch(...)
    {
        qDebug() << "Uncknown exception";
        return -1;
    }

    return 0;
}
