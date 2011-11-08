#include "fwcore/fwml.h"
#include "fwdb/dbfactory.h"
#ifdef FW_SUPPORT_POSTGRESQL
#include "fwdb/fwpg.h"
#endif // FW_SUPPORT_POSTGRESQL
#include "fwdb/fwsqlite.h"

Fw::Database* Fw::dbFactory(FwMLObject* config, QObject* parent) throw(Fw::Exception&)
{
    QByteArray driver = config->baseName();
    if(driver.isEmpty())
    {
        throw Fw::Exception("Driver name is not defined");
    }

    Fw::Database* db = 0;
    if(driver == "sqlite")
    {
        db = new FwSqlite::Database("database", parent);
    }
    #ifdef FW_SUPPORT_POSTGRESQL
    else if(driver == "postgresql")
    {
        db = new FwPg::Database("database", parent);
    }
    #endif //FW_SUPPORT_POSTGRESQL
    else
    {
        throw  Fw::Exception("Unknow driver name");
    }

    if(!db->loadData(config))
    {
        delete db;
        throw  Fw::Exception("Cannot load database config");
    }

    try
    {
        db->open();
    }
    catch(Fw::Exception& e)
    {
        delete db;
        throw e;
    }

    return db;
}

Fw::Database* Fw::dbFactory(const QString& configFile, QObject* parent) throw(Fw::Exception&)
{
    FwMLObject rootObject;
    rootObject.parseFile(configFile);
    return Fw::dbFactory(&rootObject, parent);
}
