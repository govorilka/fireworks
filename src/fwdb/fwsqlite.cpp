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
   release();
}

FwSqlite::QueryData::QueryData(FwSqlite::Database* db, const QByteArray& query) :
    BaseClass(db),
    m_stmt(0)
{
    if(db && db->m_connection)
    {
        int result = sqlite3_prepare16_v2(db->m_connection,
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

void FwSqlite::QueryData::finalize()
{
    if(m_stmt)
    {
        sqlite3_finalize(m_stmt);
        m_stmt = 0;
    }
}

bool FwSqlite::QueryData::isNull() const
{
    return m_stmt == 0;
}

void FwSqlite::QueryData::reset()
{
    if(m_stmt)
    {
        sqlite3_reset(m_stmt);
    }
}

bool FwSqlite::QueryData::step() throw (Fw::Exception&)
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
            throw Fw::Exception(m_db);
            return false;

            default:
            Q_ASSERT(false);
            return false;
        }
    }

    throw Exception("Query is null");
    return false;
}

bool FwSqlite::QueryData::columnBool(int column)
{
    return columnInt(column) != 0;
}

int FwSqlite::QueryData::columnInt(int column)
{
    if(m_stmt)
    {
        return sqlite3_column_int(m_stmt, column);
    }
    return 0;
}

QString FwSqlite::QueryData::columnText(int column)
{
    if(m_stmt)
    {
        return QString( reinterpret_cast<const QChar*>(sqlite3_column_text16(m_stmt, column)),
                        sqlite3_column_bytes16(m_stmt, column) / sizeof(QChar)
                      );
    }
    return QString();
}

FwColor FwSqlite::QueryData::columnColor(int column)
{
    return FwColor(static_cast<quint32>(columnInt(column)));
}

QUrl FwSqlite::QueryData::columnUrl(int column)
{
    QString stringUrl = columnText(column);
    if(!stringUrl.isEmpty())
    {
        return QUrl(stringUrl);
    }
    return QUrl();
}

void FwSqlite::QueryData::bindInt(int index, int value)
{
    if(m_stmt)
    {
        sqlite3_bind_int(m_stmt, index, value);
    }
}

void FwSqlite::QueryData::bindText(int index, const QString& text)
{
    if(m_stmt)
       {
           sqlite3_bind_text16(m_stmt, index, text.utf16(), (text.size()) * sizeof(QChar), SQLITE_TRANSIENT);
       }
}

void FwSqlite::QueryData::bindColor(int index, const FwColor& color)
{
    if(m_stmt)
    {
        sqlite3_bind_int(m_stmt, index, static_cast<qint32>(color.argb()));
    }
}

void FwSqlite::QueryData::bindDateTime(int index, const QDateTime& datetime)
{
    if(m_stmt)
    {
        sqlite3_bind_int(m_stmt, index, (int)datetime.toUTC().toTime_t());
    }
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
    int result = sqlite3_open_v2(param.toUtf8().data(), &m_connection, flags, 0);

    if(m_connection && result == SQLITE_OK)
    {
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

//FwSQLiteQuery FwSQLiteDatabase::query(const QString& query) throw(FwSQLiteException&)
//{
//    if(m_db)
//    {
//        sqlite3_stmt* pStmt = 0;
//        int result = sqlite3_prepare16_v2(m_db,
//                                          query.constData(),
//                                          (query.size() + 1) * sizeof(QChar),
//                                          &pStmt,
//                                          0);
//        if(result == SQLITE_OK)
//        {
//            FwSQLiteQueryData* data = new FwSQLiteQueryData(this, m_db, pStmt);
//            queries.append(data);
//            return FwSQLiteQuery(data);
//        }
//    }

//    throw FwSQLiteException(m_db);
//    return FwSQLiteQuery();
//}

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
