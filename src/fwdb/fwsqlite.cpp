#include <QtCore/qdebug.h>
#include <QtCore/qfile.h>
#include "fwsqlite.h"

FwSQLiteException::FwSQLiteException(const QString& err) throw() :
    BaseClass(),
    error(err)
{
}

FwSQLiteException::FwSQLiteException(sqlite3 *db) throw() :
    BaseClass()
{
    error = db ?
       QString::fromUtf8(sqlite3_errmsg(db)) : "Database is not open";
}

FwSQLiteException::~FwSQLiteException() throw()
{
}

const char* FwSQLiteException::what() const throw()
{
    return qPrintable(error);
}

////////////////////////////////////////////////////////////////////////////////

FwSQLiteQueryData::FwSQLiteQueryData(FwSQLiteDatabase* parent, sqlite3 *pDB, sqlite3_stmt* pStmt) :
    m_parent(0),
    db(0),
    stmt(pStmt)
{
    if(m_parent && !m_parent->queries.contains(this))
    {
        m_parent->queries.append(this);
        m_parent = parent;
        db = pDB;
    }
}

FwSQLiteQueryData::~FwSQLiteQueryData()
{
    finalize();

    if(m_parent)
    {
        m_parent->queries.removeOne(this);
        m_parent = 0;
        db = 0;
    }
}

void FwSQLiteQueryData::finalize()
{
    if(stmt)
    {
        sqlite3_finalize(stmt);
        stmt = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////

/*!
\note Used for null query only
*/
FwSQLiteQuery::FwSQLiteQuery() :
    BaseClass()
{
}

FwSQLiteQuery::FwSQLiteQuery(FwSQLiteQueryData * data) :
    BaseClass(data)
{
}

bool FwSQLiteQuery::isNull() const
{
    FwSQLiteQueryData* data = this->data();
    if(data)
    {
        return data->stmt == 0;
    }
    return true;
}

bool FwSQLiteQuery::operator==(const FwSQLiteQuery &other) const
{
    return data() == other.data();
}

bool FwSQLiteQuery::operator!=(const FwSQLiteQuery &other) const
{
    return data() != other.data();
}

bool FwSQLiteQuery::step() throw (FwSQLiteException&)
{
    FwSQLiteQueryData* data = this->data();
    if(data && data->stmt)
    {
        switch(sqlite3_step(data->stmt))
        {
        case SQLITE_ROW:
            return true;

        case SQLITE_DONE:
            return false;

        case SQLITE_ERROR:
        case SQLITE_IOERR:
            throw FwSQLiteException(data->db);
            return false;

        default:
            Q_ASSERT(false);
            return false;
        }
    }

    throw FwSQLiteException("Query is null");
    return false;
}

QString FwSQLiteQuery::columnText(int column)
{
    FwSQLiteQueryData* data = this->data();
    if(data && data->stmt)
    {
        return QString(reinterpret_cast<const QChar *>(sqlite3_column_text16(data->stmt, column)),
                       sqlite3_column_bytes16(data->stmt, column) / sizeof(QChar));
    }
    return QString();
}

int FwSQLiteQuery::columnInt(int column)
{
    FwSQLiteQueryData* data = this->data();
    if(data && data->stmt)
    {
        return sqlite3_column_int(data->stmt, column);
    }
    return 0;
}

QUrl FwSQLiteQuery::columnUrl(int column)
{
    QString stringUrl = columnText(column);
    if(!stringUrl.isEmpty())
    {
        return QUrl(stringUrl);
    }
    return QUrl();
}

bool FwSQLiteQuery::columnBool(int column)
{
    return columnInt(column) != 0;
}

FwColor FwSQLiteQuery::columnColor(int column)
{
    return FwColor(static_cast<quint32>(columnInt(column)));
}

void FwSQLiteQuery::finalize()
{
    FwSQLiteQueryData* data = this->data();
    if(data)
    {
        data->finalize();
    }
}

void FwSQLiteQuery::bindText(int index, const QString& text)
{
    FwSQLiteQueryData* data = this->data();
    if(data && data->stmt)
    {
        sqlite3_bind_text16(data->stmt, index, text.utf16(), (text.size()) * sizeof(QChar), SQLITE_TRANSIENT);
    }
}

void FwSQLiteQuery::bindInt(int index, int value)
{
    FwSQLiteQueryData* data = this->data();
    if(data && data->stmt)
    {
        sqlite3_bind_int(data->stmt, index, value);
    }
}

void FwSQLiteQuery::bindDateTime(int index, const QDateTime& datetime)
{
    FwSQLiteQueryData* data = this->data();
    if(data && data->stmt)
    {
        sqlite3_bind_int(data->stmt, index, (int)datetime.toUTC().toTime_t());
    }
}

void  FwSQLiteQuery::bindColor(int index, const  FwColor& color)
{
    FwSQLiteQueryData* data = this->data();
    if(data && data->stmt)
    {
        sqlite3_bind_int(data->stmt, index, static_cast<qint32>(color.argb()));
    }
}

void FwSQLiteQuery::reset()
{
    FwSQLiteQueryData* data = this->data();
    if(data && data->stmt)
    {
        sqlite3_reset(data->stmt);
    }
}

////////////////////////////////////////////////////////////////////////////////

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

void FwSQLiteDatabase::open(const QString& fileName, const QString& initScript) throw(FwSQLiteException&)
{
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    open(fileName, initScript, flags);
}

void FwSQLiteDatabase::open(const QString& fileName, const QString& initScript, int flags) throw(FwSQLiteException&)
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
    if(!initScript.isEmpty())
    {
        execFile(initScript);
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
            exec(QString::fromUtf8(command));
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
