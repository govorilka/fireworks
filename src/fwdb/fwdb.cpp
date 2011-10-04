#include "fwdb.h"

Fw::Exception::Exception(const Database* db) throw() :
    BaseClass()
{
    if(!db)
    {
        m_error = "Can`t connect data base";
    }
}

Fw::Exception::Exception(const QString& error) throw() :
    BaseClass()
{
    m_error = error;
}

Fw::Exception::~Exception() throw()
{
}

const char* Fw::Exception::what() const throw()
{
    return qPrintable(m_error);
}

////////////////////////////////////////////////////////////////////////////////

Fw::Query::Query()
{
}

Fw::Query::Query(QueryData * data) :
    BaseClass(data)
{
}

Fw::Query::~Query()
{
}

///////////////////////////////////////////////////////////////////////////////

Fw::QueryData::QueryData(Fw::Database* db) :
    m_db(db)
{
}

Fw::QueryData::~QueryData()
{
}

void Fw::QueryData::release()
{
    finalize();
    if(m_db)
    {
        m_db->m_queries.removeOne(this);
        m_db = 0;
    }
}

///////////////////////////////////////////////////////////////////////////////

Fw::Database::Database(QObject* parent) :
    BaseClass(parent),
    m_open(false)
{
}

Fw::Database::~Database()
{
}

Fw::Query Fw::Database::query(const QString& query) throw(Exception&)
{
    QueryData* data = createQuery(query);
    m_queries.append(data);
    return Query(data);
}

void Fw::Database::open(const QString& param) throw(Exception&)
{
    close();
    m_open = init(param);
}

void Fw::Database::close() throw()
{
    if(m_open)
    {
        foreach(QueryData* query, m_queries)
        {
            query->release();
        }
        m_queries.clear();

        m_open = false;
    }

    release();
}


////////////////////////////////////////////////////////////////////////////

Fw::DatabaseLocker::DatabaseLocker(Database *db) :
    m_db(db),
    m_lock(false)
{
}

Fw::DatabaseLocker::~DatabaseLocker()
{
    unlock();
}

bool Fw::DatabaseLocker::lock() const
{
    if(!m_lock && m_db)
    {
        m_db->m_dbLock.lockForWrite();
        m_lock = true;
    }
    return m_lock;
}

bool Fw::DatabaseLocker::tryLock() const
{
    if(!m_lock && m_db && m_db->m_dbLock.tryLockForWrite())
    {
        m_lock = true;
    }
    return m_lock;
}

void Fw::DatabaseLocker::unlock()
{
    if(m_lock)
    {
        m_db->m_dbLock.unlock();
        m_lock = false;
    }
}
