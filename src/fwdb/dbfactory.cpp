#include "fwdb/dbfactory.h"
#ifdef FW_SUPPORT_POSTGRESQL
#include "fwdb/fwpg.h"
#endif // FW_SUPPORT_POSTGRESQL
#include "fwdb/fwsqlite.h"

Fw::Database* Fw::dbFactory(QObject* parent, const QString& driver, const QString& params) throw(Fw::Exception&)
{
    QString driverLower = driver.toLower();

    Fw::Database* db = 0;
    if(driverLower == "sql")
    {
        db = new FwSqlite::Database(parent);
    }
    #ifdef FW_SUPPORT_POSTGRESQL
    else if(driverLower == "postgre")
    {
        db = new FwPg::Database(parent);
    }
    #endif //FW_SUPPORT_POSTGRESQL
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
