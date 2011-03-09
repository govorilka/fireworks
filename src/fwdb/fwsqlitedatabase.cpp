#include <QtCore/qdebug.h>
#include <QtCore/qfile.h>

#include "fwsqlitedatabase.h"

FwSQLiteDatabase::FwSQLiteDatabase(QObject* parent) :
    BaseClass(parent),
    m_db(0),
    m_beginTransaction(false)
{
}

FwSQLiteDatabase::~FwSQLiteDatabase()
{
    close();
}

void FwSQLiteDatabase::open(const QString& fileName) throw(FwSQLiteException&)
{
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    open(fileName, flags);
}

void FwSQLiteDatabase::open(const QString& fileName, int flags) throw(FwSQLiteException&)
{
    if(m_db)
    {
        close();
    }

    int result = sqlite3_open_v2(fileName.toUtf8().data(), &m_db, flags, 0);
    if(result != SQLITE_OK)
    {
        QString error = QString::fromUtf8(sqlite3_errmsg(m_db));
        sqlite3_close(m_db);
        m_db = 0;
        throw FwSQLiteException(error);
    }
}

void FwSQLiteDatabase::close()
{
    if(m_db)
    {
        foreach(FwSQLiteQueryData* query, queries)
        {
            query->finalize();
            query->m_parent = 0;
            query->db = 0;
        }
        queries.clear();

        sqlite3_close(m_db);
        m_db = 0;
    }
}

FwSQLiteQuery FwSQLiteDatabase::query(const QString& query) throw(FwSQLiteException&)
{
    if(m_db)
    {
        sqlite3_stmt* pStmt = 0;
        int result = sqlite3_prepare16_v2(m_db,
                                          query.constData(),
                                          (query.size() + 1) * sizeof(QChar),
                                          &pStmt,
                                          0);
        if(result == SQLITE_OK)
        {
            return FwSQLiteQuery(new FwSQLiteQueryData(this, m_db, pStmt));
        }
    }

    throw FwSQLiteException(m_db);
    return FwSQLiteQuery();
}

void FwSQLiteDatabase::exec(const QString& query) throw(FwSQLiteException&)
{
    if(!m_db)
    {
        throw FwSQLiteException(m_db);
    }
    FwSQLiteQuery q = this->query(query);
    while(q.step());
}

void FwSQLiteDatabase::execFile(const QString& fileName) throw(FwSQLiteException&)
{
    QFile sqlFile(fileName);
    execFile(&sqlFile);
}

void FwSQLiteDatabase::execFile(QIODevice *device) throw(FwSQLiteException&)
{
    if(!device->open(QIODevice::ReadOnly))
    {
        throw FwSQLiteException(device->errorString());
    }

    beginTransaction();

    QList<QByteArray> commands = device->readAll().simplified().split(';');
    foreach(QByteArray command, commands)
    {
        if(!command.isEmpty())
        {
            exec(command);
        }
    }

    commit();
}

void FwSQLiteDatabase::beginTransaction() throw(FwSQLiteException&)
{
    if(!m_beginTransaction)
    {
        exec("BEGIN;");
        m_beginTransaction = true;
    }
}

void FwSQLiteDatabase::commit() throw(FwSQLiteException&)
{
    if(m_beginTransaction)
    {
        m_beginTransaction = false;
        exec("COMMIT;");
    }
}

void FwSQLiteDatabase::rollback() throw(FwSQLiteException&)
{
    if(m_beginTransaction)
    {
        m_beginTransaction = false;
        exec("ROLLBACK;");
    }
}

int FwSQLiteDatabase::lastInsertKey() const
{
    if(m_db)
    {
        return sqlite3_last_insert_rowid(m_db);
    }
    return 0;
}

void FwSQLiteDatabase::reindex(const QString& indexName) throw(FwSQLiteException&)
{
    if(!m_db)
    {
        throw FwSQLiteException(m_db);
    }
    FwSQLiteQuery reindexQuery = query(QString("REINDEX %1").arg(indexName));
    reindexQuery.step();
}

//////////////////////////////////////////////////////////////////////////

FwSQLiteDBLock::FwSQLiteDBLock(FwSQLiteDatabase *db) :
    m_db(db),
    m_lock(false)
{
}

FwSQLiteDBLock::~FwSQLiteDBLock()
{
    unlock();
}

bool FwSQLiteDBLock::lock() const
{
    if(!m_lock && m_db)
    {
        m_db->m_dbLock.lockForWrite();
        m_lock = true;
    }
    return m_lock;
}

bool FwSQLiteDBLock::tryLock() const
{
    if(!m_lock && m_db && m_db->m_dbLock.tryLockForWrite())
    {
        m_lock = true;
    }
    return m_lock;
}

void FwSQLiteDBLock::unlock()
{
    if(m_lock)
    {
        m_db->m_dbLock.unlock();
        m_lock = false;
    }
}
