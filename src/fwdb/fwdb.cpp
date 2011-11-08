#include <QtCore/qdebug.h>

#include "fwdb.hpp"

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
    if(!(m_exec = (m_exec ? doNext() : doExec())))
    {
        doReset();
    }
    return m_exec;
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

Fw::Database::Database(const QByteArray& name, QObject* parent) :
    QObject(parent),
    BaseClass(name),
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
        lquery.step();
        m_begin_transaction = false;
    }
}

void Fw::Database::rollback() throw(Exception&)
{
    if(m_begin_transaction)
    {
        Query lquery = query("ROLLBACK");
        lquery.step();
        m_begin_transaction = false;
    }
}

void Fw::Database::open() throw(Exception&)
{
    close();
    init();
    m_open = true;
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

        release();

        m_open = false;
    }
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
        throw Fw::Exception(device->errorString().toUtf8());
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
