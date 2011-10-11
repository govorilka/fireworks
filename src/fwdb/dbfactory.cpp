#include "fwdb/fwdb.h"
#ifdef FW_SUPPORT_POSTGRESQL
#include "fwdb/fwpg.h"
#endif
#include "fwdb/fwsqlite.h"

Fw::Database* Fw::dbFactory(QObject* parent, const QString& driver, const QString& params)
{
    QString driverLower = driver.toLower();
    if(driverLower == "sql")
    {
        return new FwSqlite::Database(parent);
    }

#ifdef FW_SUPPORT_POSTGRESQL
    if(driverLower == "postgre")
    {
        return new FwPg::Database(parent);
    }
#endif

    return 0;
}
