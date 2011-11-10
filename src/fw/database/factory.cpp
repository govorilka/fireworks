#include "factory.hpp"

#include "fwcore/fwml.h"

#include "fw/database/sqlite/driver.hpp"

#ifdef FW_SUPPORT_POSTGRESQL
#include "fwdb/fwpg.h"
#endif // FW_SUPPORT_POSTGRESQL

Fw::Database::Driver* Fw::Database::factory(FwMLObject* config) throw(Fw::Exception&)
{
    QByteArray driver = config->baseName();
    if(driver.isEmpty())
    {
        throw Fw::Exception("Driver name is not defined");
    }

    Fw::Database::Driver* db = 0;
    if(driver == "sqlite")
    {
        return new SQLite::Driver("database");
    }
    #ifdef FW_SUPPORT_POSTGRESQL
    else if(driver == "postgresql")
    {
        return new PostgreSQL::Driver("database");
    }
    #endif //FW_SUPPORT_POSTGRESQL

    throw  Fw::Exception("Unknow driver name");
    return 0;
}

Fw::Database::Driver* Fw::Database::factory(const QString& configFile) throw(Fw::Exception&)
{
    FwMLObject rootObject;
    rootObject.parseFile(configFile);
    return factory(&rootObject);
}
