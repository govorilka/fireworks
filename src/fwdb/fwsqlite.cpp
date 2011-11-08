#include <QtCore/qdebug.h>

#include "fwcore/fwml.h"
#include "fwsqlite.h"

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

    throw Fw::Exception(db->lastError().toUtf8());
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

bool FwSqlite::QueryData::doExec() throw (Fw::Exception&)
{
    return doNext();
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
            throw Fw::Exception(static_cast<FwSqlite::Database*>(m_db)->lastError().toUtf8());
            return false;

            default:
            Q_ASSERT(false);
            return false;
        }
    }

    throw Fw::Exception("Query is null");
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
    doBindInt(index, static_cast<int>(dateTime.toUTC().toTime_t()));
}

void FwSqlite::QueryData::doBindDate(int index, const QDate& date) throw(Fw::Exception&)
{
    doBindDateTime(index, QDateTime(date));
}

void FwSqlite::QueryData::doBindTime(int index, const QTime& time) throw(Fw::Exception&)
{
    doBindInt(index, time.second() + time.minute() * 60 + time.hour() * 3600);
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

QDateTime FwSqlite::QueryData::doColumnDateTime(int column) const
{
    return QDateTime::fromTime_t(doColumnInt(column));
}

QDate FwSqlite::QueryData::doColumnDate(int column) const
{
    return QDateTime::fromTime_t(doColumnInt(column)).date();
}

QTime FwSqlite::QueryData::doColumnTime(int column) const
{
    int time = doColumnInt(column);
    int h = time / 3600;
    time -= h * 3600;
    int m = time / 60;
    int s = time - m * 60;
    return QTime(h, m, s);
}

////////////////////////////////////////////////////////////////////////////////

FwSqlite::Database::Database(const QByteArray& name, QObject* parent) :
    BaseClass(name, parent),
    m_connection(0)
{
}

FwSqlite::Database::~Database()
{
    close();
}

void FwSqlite::Database::init() throw(Fw::Exception&)
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

bool FwSqlite::Database::loadData(FwMLObject* object)
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

QString FwSqlite::Database::lastError() const
{
    if(this && m_connection)
    {
        return QString(sqlite3_errmsg(m_connection));
    }

    return "No database connection";

}
