#include "fwdb/dbfactory.h"
#include "fwdb/fwpg.h"
#include "fwdb/fwsqlite.h"

Fw::Database* Fw::dbFactory(QObject* parent, const QString& driver, const QString& params) throw(Fw::Exception&)
{
    QString driverLower = driver.toLower();

    Fw::Database* db = 0;
    if(driverLower == "sql")
    {
        db = new FwSqlite::Database(parent);
    }
    else if(driverLower == "postgre")
    {
        db = new FwPg::Database(parent);
    }

    if(db)
    {
        try
        {
        db->open(params);
        }
        catch(Exception& e)
        {
            delete db;
            db = 0;
            throw e;
        }
    }

    return db;
}
