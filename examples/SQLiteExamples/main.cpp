#include <QtCore/QCoreApplication>
#include <QtCore/qdebug.h>

#include "fwdb/fwdb.h"

int main(int argc, char *argv[])
{
    try
    {
        Fw::Database* m_db = Fw::dbFactory(0, "sql","./iptv001.db");
        Fw::Query query = m_db->query("SELECT tvprogramcategoryid,caption FROM tvprogramcategory;");
        while(query.step())
        {
            qDebug() << "=========================================";
            qDebug() << query.columnInt(0) << query.columnText(1);
            qDebug() << "=========================================";
        }
    }
    catch(Fw::Exception& e)
    {
        qDebug() << e.what();
    }

    return 0;
}
