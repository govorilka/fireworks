#include "fw/database/sqlite/querydata.hpp"
#include "fw/database/sqlite/driver.hpp"

void Fw::Database::SQLite::Query::closeQuery()
{
    if(m_stmt)
    {
        sqlite3_finalize(m_stmt);
        m_stmt = 0;
    }
}

Fw::Database::SQLite::Query::Query(const DriverPtr& driver, sqlite3_stmt* stmt) :
    BaseClass(driver),
    m_stmt(stmt)
{
}

Fw::Database::SQLite::Query::~Query()
{
    closeQuery();
}

bool Fw::Database::SQLite::Query::doExec() throw (const Fw::Exception&)
{
    return doNext();
}

bool Fw::Database::SQLite::Query::doNext() throw (const Fw::Exception&)
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
            throw Fw::Exception(static_cast<Driver*>(driver())->lastError().toUtf8());
            return false;

            default:
            Q_ASSERT(false);
            return false;
        }
    }

    throw Fw::Exception("Query is null");
    return false;
}

void Fw::Database::SQLite::Query::doReset()
{
    if(m_stmt)
    {
        sqlite3_reset(m_stmt);
    }
}

void Fw::Database::SQLite::Query::bindInt(int index, int value) throw(const Fw::Exception&)
{
    if(m_stmt)
    {
        sqlite3_bind_int(m_stmt, index, value);
    }
}

void Fw::Database::SQLite::Query::bindText(int index, const QString& text) throw(const Fw::Exception&)
{
    if(m_stmt)
       {
           sqlite3_bind_text16(m_stmt, index, text.utf16(), (text.size()) * sizeof(QChar), SQLITE_TRANSIENT);
       }
}

void Fw::Database::SQLite::Query::bindDateTime(int index, const QDateTime& dateTime) throw(const Fw::Exception&)
{
    bindInt(index, static_cast<int>(dateTime.toUTC().toTime_t()));
}

void Fw::Database::SQLite::Query::bindDate(int index, const QDate& date) throw(const Fw::Exception&)
{
    bindDateTime(index, QDateTime(date));
}

void Fw::Database::SQLite::Query::bindTime(int index, const QTime& time) throw(const Fw::Exception&)
{
    bindInt(index, time.second() + time.minute() * 60 + time.hour() * 3600);
}

bool Fw::Database::SQLite::Query::columnBool(int column) const throw(const Fw::Exception&)
{
    return columnInt(column) != 0;
}

int Fw::Database::SQLite::Query::columnInt(int column) const throw(const Fw::Exception&)
{
    if(m_stmt)
    {
        return sqlite3_column_int(m_stmt, column);
    }
    return 0;
}

QString Fw::Database::SQLite::Query::columnText(int column) const throw(const Fw::Exception&)
{
    if(m_stmt)
    {
        return QString( reinterpret_cast<const QChar*>(sqlite3_column_text16(m_stmt, column)),
                        sqlite3_column_bytes16(m_stmt, column) / sizeof(QChar)
                      );
    }
    return QString();
}

FwColor Fw::Database::SQLite::Query::columnColor(int column) const throw(const Fw::Exception&)
{
    return FwColor(static_cast<quint32>(columnInt(column)));
}

QUrl Fw::Database::SQLite::Query::columnUrl(int column) const throw(const Fw::Exception&)
{
    QString stringUrl = columnText(column);
    if(!stringUrl.isEmpty())
    {
        return QUrl(stringUrl);
    }
    return QUrl();
}

QDateTime Fw::Database::SQLite::Query::columnDateTime(int column) const throw(const Fw::Exception&)
{
    return QDateTime::fromTime_t(columnInt(column));
}

QDate Fw::Database::SQLite::Query::columnDate(int column) const throw(const Fw::Exception&)
{
    return QDateTime::fromTime_t(columnInt(column)).date();
}

QTime Fw::Database::SQLite::Query::columnTime(int column) const throw(const Fw::Exception&)
{
    int time = columnInt(column);
    int h = time / 3600;
    time -= h * 3600;
    int m = time / 60;
    int s = time - m * 60;
    return QTime(h, m, s);
}
