#include <QtCore/qdebug.h>

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
           return(m_exec = doExec());
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
    QObject(parent),
    BaseClass("DataBase"),
    m_open(false),
    m_begin_transaction(false)
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

void Fw::Database::beginTransaction() throw(Exception&)
{
    if(!m_begin_transaction)
    {
        Query lquery = query("BEGIN");
        qDebug() << "BEGIN";
        lquery.step();
        m_begin_transaction = true;
    }
}

void Fw::Database::commit() throw(Exception&)
{
    if(m_begin_transaction)
    {
        Query lquery = query("COMMIT");
        qDebug() << "COMMIT";
        lquery.step();
        m_begin_transaction = false;
    }
}

void Fw::Database::rollback() throw(Exception&)
{
    if(m_begin_transaction)
    {
        Query lquery = query("ROLLBACK");
        qDebug() << "ROLLBACK";
        lquery.step();
        m_begin_transaction = false;
    }
}

void Fw::Database::open(FwMLObject* object, bool* createdDB) throw(Exception&)
{
    close();
    m_open = init(object, createdDB);
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

void Fw::Database::reindex(const QString& indexName) throw(Fw::Exception&)
{
    Query reindexQuery = query(QString("REINDEX %1").arg(indexName));
    reindexQuery.step();
}

void Fw::Database::execFile(const QString& fileName) throw(Fw::Exception&)
{
    QFile sqlFile(fileName);
    execFile(&sqlFile);
}

void Fw::Database::execFile(QIODevice* device) throw(Fw::Exception&)
{
    if(!device->open(QIODevice::ReadOnly))
    {
        throw Fw::Exception(device->errorString());
    }

    beginTransaction();

    QList<QByteArray> commands = device->readAll().simplified().split(';');
    foreach(QByteArray command, commands)
    {
        if(!command.isEmpty())
        {
            qDebug() << command;
            Fw::Query q = this->query(QString::fromUtf8(command));
            while(q.step());
        }
    }

    commit();
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
