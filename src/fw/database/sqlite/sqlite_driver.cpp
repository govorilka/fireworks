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

bool Fw::Database::SQLite::Driver::loadData(FwMLObject* object) throw(const Fw::Exception&)
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

void Fw::Database::SQLite::Driver::open() throw(const Fw::Exception&)
{
    const int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if(m_path.isEmpty())
    {
        throw Fw::Exception("Path to database is empty");
    }

    bool needCreate = !QFile::exists(m_path);
    if(needCreate && !QFile::exists(m_initPath))
    {
        throw Fw::Exception("Init path is not set");
    }

    int result = sqlite3_open_v2(m_path.toUtf8().data(), &m_connection, flags, 0);
    if(m_connection &&
       result == SQLITE_OK &&
       sqlite3_exec(m_connection, "PRAGMA FOREIGN_KEYS = ON;", 0, 0, 0) == SQLITE_OK &&
       sqlite3_exec(m_connection, "PRAGMA ENCODING=\"UTF-8\";", 0, 0, 0) == SQLITE_OK)
    {
        if(needCreate)
        {
            execFile(m_initPath);
        }
        return;
    }

    throw Fw::Exception(QString("Cannot open database: %1").arg(lastError()).toUtf8());
}

void Fw::Database::SQLite::Driver::close() throw()
{
    if(m_connection)
    {
        sqlite3_close(m_connection);
        m_connection = 0;
    }
}

Fw::Database::QueryPtr Fw::Database::SQLite::Driver::createQuery(const Fw::Database::DriverPtr& driver, const QString& query) throw(const Fw::Exception&)
{
    if(m_connection)
    {
        sqlite3_stmt* stmt = 0;
        int result = sqlite3_prepare16_v2(m_connection,
                                          query.constData(),
                                         (query.size() + 1) * sizeof(QChar),
                                         &stmt,
                                         0);
        if(result == SQLITE_OK)
        {
            return Fw::Database::QueryPtr(new Query(driver, stmt));
        }
    }

    throw Fw::Exception(lastError().toUtf8());
}

QString Fw::Database::SQLite::Driver::lastError() const
{
    if(this && m_connection)
    {
        return QString(sqlite3_errmsg(m_connection));
    }

    return BaseClass::lastError();

}

bool Fw::Database::SQLite::Driver::isOpen() const
{
    return m_connection;
}
int Fw::Database::SQLite::Driver::lastInsertKey()
{
    if(m_connection)
    {
        return sqlite3_last_insert_rowid(m_connection);
    }
    return 0;
}
