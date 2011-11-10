#include "fwcore/fwml.h"

#include "fw/database/sqlite/driver.hpp"
#include "fw/database/sqlite/querydata.hpp"

Fw::Database::SQLite::Driver::Driver(const QByteArray& name) :
    BaseClass(name),
    m_connection(0)
{
}

Fw::Database::SQLite::Driver::~Driver()
{
    close();
}

bool Fw::Database::SQLite::Driver::loadData(FwMLObject* object)
{
    FwMLString* pathNode = object->attribute("path")->cast<FwMLString>();
    if(pathNode)
    {
        setPath(pathNode->toQString());
    }
    FwMLString* initPathNode = object->attribute("initPath")->cast<FwMLString>();
    if(initPathNode)
    {
        setInitPath(initPathNode->toQString());
    }
    return true;
}

Fw::Database::QueryData* Fw::Database::SQLite::Driver::query(const QString& q) const throw(Fw::Exception&)
{
    if(m_connection)
    {
        sqlite3_stmt* stmt = 0;
        int result = sqlite3_prepare16_v2(m_connection,
                                          q.constData(),
                                         (q.size() + 1) * sizeof(QChar),
                                         &stmt,
                                         0);
        if(result == SQLITE_OK)
        {
            return new QueryData(const_cast<Driver*>(this), stmt);
        }
    }

    throw Fw::Exception(lastError().toUtf8());
    return 0;
}
