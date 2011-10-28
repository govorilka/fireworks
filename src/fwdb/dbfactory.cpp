#include "fwcore/fwml.h"
#include "fwdb/dbfactory.h"
#ifdef FW_SUPPORT_POSTGRESQL
#include "fwdb/fwpg.h"
#endif // FW_SUPPORT_POSTGRESQL
#include "fwdb/fwsqlite.h"

Fw::Database* Fw::dbFactory(QObject* parent, const QString& driver, FwMLObject* config, bool* createdDB) throw(Fw::Exception&)
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
        db->open(config, createdDB);
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

Fw::Database* Fw::dbFactory(QObject *parent, const QString &configFile, bool* createdDB) throw(Fw::Exception&)
{
    QFile fwmlData(QDir::toNativeSeparators(configFile));
    if(!fwmlData.exists())
    {
        qWarning(qPrintable(QString("File %1 not found").arg(configFile)));
        return 0;
    }

    QString error;
    FwMLObject rootObject;
    if(!rootObject.parse(&fwmlData, &error))
    {
        qWarning(qPrintable(QString("FwML error in %1 file: %2").arg(configFile).arg(error)));
        return 0;
    }

    FwMLObject* databaseSection = rootObject.attribute("database")->cast<FwMLObject>();
    FwMLString* driver = databaseSection->attribute("driver")->cast<FwMLString>();

    if(driver)
    {
        try
        {
            return dbFactory(parent, driver->value(), databaseSection, createdDB);
        }
        catch(Exception& e)
        {
            throw e;
        }
    }
    return 0;
}
