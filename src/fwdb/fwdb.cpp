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

Fw::Query::Query(QueryData * data) :
    BaseClass(data, &doDeleteQueryData)
{
}

Fw::Query::~Query()
{
}

Fw::QueryData* Fw::Query::getQueryData() const throw (Fw::Exception&)
{
    QueryData* data = this->data();
    if(!data || !data->m_db)
    {
        throw Fw::Exception("Query is null");
    }
    return data;
}

Fw::QueryData* Fw::Query::getBindQueryData() const throw (Fw::Exception&)
{
    QueryData* data = getQueryData();
    if(data->m_exec)
    {
        throw Fw::Exception("Query is execite now");
    }
    return data;
}

void Fw::Query::doDeleteQueryData(QueryData* data)
{
    data->release();
    delete data;
    data = 0;
}

///////////////////////////////////////////////////////////////////////////////

Fw::QueryData::QueryData(Fw::Database* db) :
    m_db(db),
    m_exec(false)
{
}

bool Fw::QueryData::step() throw (Fw::Exception&)
{
    if(!m_exec)
    {
        doExec();
        m_exec = true;
        return true;
    }

    if(doNext())
    {
        return true;
    }

    reset();
    return false;
}

void Fw::QueryData::reset()
{
    if(m_exec)
    {
        doReset();
        m_exec = false;
    }
}

Fw::QueryData::~QueryData()
{
}

void Fw::QueryData::release()
{
    if(m_db)
    {
        reset();
        doFinalize();
        m_db->m_queries.removeOne(this);
        m_db = 0;
    }
}

///////////////////////////////////////////////////////////////////////////////

Fw::Database::Database(QObject* parent) :
    BaseClass(parent),
    m_open(false),
    m_begin_transaction(false)
{
}

Fw::Database::~Database()
{
}

Fw::Query Fw::Database::query(const QString& query) throw(Exception&)
{
    beginTransaction();

    QueryData* data = createQuery(query);
    m_queries.append(data);
    return Query(data);
}

void Fw::Database::beginTransaction() throw(Exception&)
{
    if(!m_begin_transaction)
    {
        m_begin_transaction = true;
        try
        {
            Query lquery = query("BEGIN");
        }
        catch(Exception& e)
        {
            m_begin_transaction = false;
            throw e;
        }
    }
}

void Fw::Database::commit() throw(Exception&)
{
    if(m_begin_transaction)
    {
        m_begin_transaction = false;
        Query lquery = query("COMMIT");
    }
}

void Fw::Database::rollback() throw(Exception&)
{
    if(m_begin_transaction)
    {
        m_begin_transaction = false;
        Query lquery = query("ROLLBACK");
    }
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
        rollback();

        foreach(QueryData* query, m_queries)
        {
            query->release();
        }
        m_queries.clear();

        m_open = false;
    }

    release();
}

int Fw::Database::lastInsertKey()
{
    //TODO
}

void Fw::Database::reindex(const QString& indexName) throw(Fw::Exception&)
{
    //TODO
}

void Fw::Database::execFile(const QString& fileName) throw(Fw::Exception&)
{
    //TODO
}

void Fw::Database::execFile(QIODevice* device) throw(Fw::Exception&)
{
    //TODO
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
