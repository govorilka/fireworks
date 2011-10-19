//#include <QtCore/qdebug.h>
//#include <QtCore/qfile.h>

#include "fwsqlite.h"

FwSqlite::Exception::Exception(const Database* db) throw() :
    BaseClass(db)
{
    if(db && db->m_connection)
    {
        m_error = sqlite3_errmsg(db->m_connection);
    }
    else
    {
        m_error = "No database connection";
    }
}

FwSqlite::Exception::Exception(const QString& error) throw() :
    BaseClass(error)
{
}

FwSqlite::Exception::~Exception() throw()
{
}

////////////////////////////////////////////////////////////////////////////////

FwSqlite::QueryData::~QueryData()
{
}

FwSqlite::QueryData::QueryData(FwSqlite::Database* db, const QByteArray& query) throw(Fw::Exception&) :
    BaseClass(db),
    m_stmt(0)
{
    if(db && db->m_connection)
    {
        int result = sqlite3_prepare_v2(db->m_connection,
                                          query.constData(),
                                          (query.size() + 1) * sizeof(QChar),
                                          &m_stmt,
                                          0);
        if(result == SQLITE_OK)
        {
            return;
        }
    }

    throw FwSqlite::Exception(db);
}

bool FwSqlite::QueryData::isNull() const
{
    return m_stmt == 0;
}

bool FwSqlite::QueryData::operator==(const QueryData& other) const
{
    return m_db == other.m_db && m_stmt == other.m_stmt;
}

bool FwSqlite::QueryData::operator!=(const QueryData& other) const
{
    return !operator==(other);
}

void FwSqlite::QueryData::doExec() throw (Fw::Exception&)
{
    doNext();// STUB!!!!!!!!!!!!!!!!!!!!!!!!!!
}

bool FwSqlite::QueryData::doNext() throw (Fw::Exception&)
{
    if(m_stmt)
    {
        switch(sqlite3_step(m_stmt))
        {
            case SQLITE_ROW:
            return true;

            case SQLITE_DONE:
            return false;

            case SQLITE_ERROR:
            case SQLITE_IOERR:
            case SQLITE_CONSTRAINT:
            throw FwSqlite::Exception(reinterpret_cast<FwSqlite::Database*>(m_db));
            return false;

            default:
            Q_ASSERT(false);
            return false;
        }
    }

    throw Exception("Query is null");
    return false;
}

void FwSqlite::QueryData::doReset()
{
    if(m_stmt)
    {
        sqlite3_reset(m_stmt);
    }
}

void FwSqlite::QueryData::doFinalize()
{
    if(m_stmt)
    {
        sqlite3_finalize(m_stmt);
        m_stmt = 0;
    }
}

void FwSqlite::QueryData::doBindInt(int index, int value) throw(Fw::Exception&)
{
    if(m_stmt)
    {
        sqlite3_bind_int(m_stmt, index, value);
    }
}
void FwSqlite::QueryData::doBindText(int index, const QString& text) throw(Fw::Exception&)
{
    if(m_stmt)
       {
           sqlite3_bind_text16(m_stmt, index, text.utf16(), (text.size()) * sizeof(QChar), SQLITE_TRANSIENT);
       }
}
void FwSqlite::QueryData::doBindDateTime(int index, const QDateTime& dateTime) throw(Fw::Exception&)
{
    if(m_stmt)
    {
        sqlite3_bind_int(m_stmt, index, (int)dateTime.toUTC().toTime_t());
    }
}

bool FwSqlite::QueryData::doColumnBool(int column) const
{
    return doColumnInt(column) != 0;
}
int FwSqlite::QueryData::doColumnInt(int column) const
{
    if(m_stmt)
    {
        return sqlite3_column_int(m_stmt, column);
    }
    return 0;
}

QString FwSqlite::QueryData::doColumnText(int column) const
{
    if(m_stmt)
    {
        return QString( reinterpret_cast<const QChar*>(sqlite3_column_text16(m_stmt, column)),
                        sqlite3_column_bytes16(m_stmt, column) / sizeof(QChar)
                      );
    }
    return QString();
}

FwColor FwSqlite::QueryData::doColumnColor(int column) const
{
    return FwColor(static_cast<quint32>(doColumnInt(column)));
}

QUrl FwSqlite::QueryData::doColumnUrl(int column) const
{
    QString stringUrl = doColumnText(column);
    if(!stringUrl.isEmpty())
    {
        return QUrl(stringUrl);
    }
    return QUrl();
}

////////////////////////////////////////////////////////////////////////////////

FwSqlite::Database::Database(QObject* parent) :
    BaseClass(parent),
    m_connection(0)
{
}

FwSqlite::Database::~Database()
{
    close();
}

bool FwSqlite::Database::init(const QString& param) throw(Fw::Exception&)
{
    const int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;

    QStringList parameters = param.split('|');
    bool isCreated = QFile::exists(parameters.at(0));

    int result = sqlite3_open_v2(parameters.takeFirst().toUtf8().data(), &m_connection, flags, 0);
    if(m_connection && 
       result == SQLITE_OK &&
       sqlite3_exec(m_connection, "PRAGMA FOREIGN_KEYS = ON;", 0, 0, 0) == SQLITE_OK &&
       sqlite3_exec(m_connection, "PRAGMA ENCODING=\"UTF-8\";", 0, 0, 0) == SQLITE_OK)
    {
        if(!parameters.isEmpty() && !isCreated)
        {
            execFile(parameters.takeFirst());
        }
        return true;
    }

    release();
    throw(Exception(this));
    return false;
}

void FwSqlite::Database::release() throw()
{
    if(m_connection)
    {
        sqlite3_close(m_connection);
        m_connection = 0;
    }
}

int FwSqlite::Database::lastInsertKey()
{
    if(m_connection)
    {
        return sqlite3_last_insert_rowid(m_connection);
    }
    return 0;
}

Fw::QueryData* FwSqlite::Database::createQuery(const QString& query) throw(Fw::Exception&)
{
    return new FwSqlite::QueryData(this, query.toUtf8());
}

//void FwSQLiteDatabase::exec(const QString& query) throw(FwSQLiteException&)
//{
//    if(!m_db)
//    {
//        throw FwSQLiteException(m_db);
//    }
//    FwSQLiteQuery q = this->query(query);
//    while(q.step());
//}

//void FwSQLiteDatabase::execFile(const QString& fileName) throw(FwSQLiteException&)
//{
//    QFile sqlFile(fileName);
//    execFile(&sqlFile);
//}

//void FwSQLiteDatabase::execFile(QIODevice *device) throw(FwSQLiteException&)
//{
//    if(!device->open(QIODevice::ReadOnly))
//    {
//        throw FwSQLiteException(device->errorString());
//    }

//    beginTransaction();

//    QList<QByteArray> commands = device->readAll().simplified().split(';');
//    foreach(QByteArray command, commands)
//    {
//        if(!command.isEmpty())
//        {
//            exec(QString::fromUtf8(command));
//        }
//    }

//    commit();
//}

//void FwSQLiteDatabase::beginTransaction() throw(FwSQLiteException&)
//{
//    if(!m_beginTransaction)
//    {
//        exec("BEGIN;");
//        m_beginTransaction = true;
//    }
//}

//void FwSQLiteDatabase::commit() throw(FwSQLiteException&)
//{
//    if(m_beginTransaction)
//    {
//        m_beginTransaction = false;
//        exec("COMMIT;");
//    }
//}

//void FwSQLiteDatabase::rollback() throw(FwSQLiteException&)
//{
//    if(m_beginTransaction)
//    {
//        m_beginTransaction = false;
//        exec("ROLLBACK;");
//    }
//}

//int FwSQLiteDatabase::lastInsertKey() const
//{
//    if(m_db)
//    {
//        return sqlite3_last_insert_rowid(m_db);
//    }
//    return 0;
//}

//void FwSQLiteDatabase::reindex(const QString& indexName) throw(FwSQLiteException&)
//{
//    if(!m_db)
//    {
//        throw FwSQLiteException(m_db);
//    }
//    FwSQLiteQuery reindexQuery = query(QString("REINDEX %1").arg(indexName));
//    reindexQuery.step();
//}
