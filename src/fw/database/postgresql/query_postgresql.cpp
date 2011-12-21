#include "fw/database/postgresql/query_postgresql.hpp"
#include "fw/database/postgresql/driver_postgresql.hpp"

bool Fw::Database::PostgreSQL::Query::doExec() throw (const Fw::Exception&)
{
    closeQuery(); //close query if exist

    Driver* drv = static_cast<Driver*>(driver());
    if(drv == 0 || drv->connection() == 0)
    {
        throw Fw::Exception(drv->lastError());
    }

    PGconn* connection = drv->connection();

    //make query
    m_result = PQexec(connection, toUtf8());
    ExecStatusType status = PQresultStatus(m_result);
    if (status == PGRES_FATAL_ERROR || status == PGRES_NONFATAL_ERROR)
    {
        closeQuery();
        throw Fw::Exception(drv->lastError());
    }

    m_currRow = 0;
    drv->setLastInsertKey(0);

    const Oid lastRow = PQoidValue(m_result);
    if(lastRow != InvalidOid)
    {
        drv->setLastInsertKey(lastRow);
    }

    m_countRow = PQntuples(m_result);
    return m_countRow;
}

bool Fw::Database::PostgreSQL::Query::doNext() throw (const Fw::Exception&)
{
    if(m_countRow == 0)
    {
        throw Fw::Exception("Row count is null");
    }

    ++m_currRow;
    if(m_currRow < m_countRow)
    {
        return true;
    }
    else
    {
        doReset();
        return false;
    }
}

void Fw::Database::PostgreSQL::Query::doReset()
{
    m_currRow = 0;
}

void Fw::Database::PostgreSQL::Query::closeQuery()
{
    if(m_result)
    {
        PQclear(m_result);
        m_result = 0;
    }
    m_countRow = (m_currRow = 0);
}

Fw::Database::PostgreSQL::Query::Query(const Fw::Database::DriverPtr& driver, const Fw::Database::PostgreSQL::TokenVector& query) :
    BaseClass(driver),
    m_result(0),
    m_tokens(query),
    m_countRow(0),
    m_currRow(0),
    m_lastInsertRowId(0)
{
}

Fw::Database::PostgreSQL::Query::~Query()
{
    closeQuery();
}

bool Fw::Database::PostgreSQL::Query::operator==(const Query& other) const
{
    return driver() == other.driver() && m_result == other.m_result;
}

bool Fw::Database::PostgreSQL::Query::operator!=(const Query& other) const
{
    return driver() != other.driver() && m_result != other.m_result;
}

void Fw::Database::PostgreSQL::Query::bindNull(int index) throw(const Fw::Exception&)
{
    bindByteArray(index, "NULL");
}

void Fw::Database::PostgreSQL::Query::bindBool(int index, bool value) throw(const Fw::Exception&)
{
    bindByteArray(index, value ? "true" : "false");
}

void Fw::Database::PostgreSQL::Query::bindInt(int index, int value) throw(const Fw::Exception&)
{
    bindByteArray(index, QByteArray::number(value));
}

void Fw::Database::PostgreSQL::Query::bindUInt(int index, uint value) throw(const Fw::Exception&)
{
    bindByteArray(index, QByteArray::number(value));
}

void Fw::Database::PostgreSQL::Query::bindDouble(int index, double value) throw(const Fw::Exception&)
{
    bindByteArray(index, QByteArray::number(value, 'f', 12));
}

void Fw::Database::PostgreSQL::Query::bindText(int index, const QString& text) throw(const Fw::Exception&)
{
    if(text.isEmpty())
    {
        return bindNull(index);
    }

    QString textLocal(text);
    textLocal.replace('\'', "\'\'");
    bindByteArray(index, "'" + textLocal.toUtf8() + "'");
}

void Fw::Database::PostgreSQL::Query::bindDateTime(int index, const QDateTime& dateTime) throw(const Fw::Exception&)
{
    bindDouble(index, dateTime.toTime_t());
}

void Fw::Database::PostgreSQL::Query::bindDate(int index, const QDate& date) throw(const Fw::Exception&)
{
    bindText(index, date.toString(Qt::ISODate));
}

void Fw::Database::PostgreSQL::Query::bindTime(int index, const QTime& time) throw(const Fw::Exception&)
{
    bindText(index, time.toString(Qt::ISODate));
}

void Fw::Database::PostgreSQL::Query::bindByteArray(int index, const QByteArray& text)
{
    for(Fw::Database::PostgreSQL::TokenVector::Iterator iter = m_tokens.begin(); iter != m_tokens.end(); ++iter)
    {
        QueryToken& token = *iter;
        if(token.param == index)
        {
            token.value = text;
        }
    }
}

bool Fw::Database::PostgreSQL::Query::columnBool(int column) const throw(const Fw::Exception&)
{
    if(PQgetisnull(m_result, m_currRow, column) == 0)
    {
        if('t' == *PQgetvalue(m_result, m_currRow, column))
        {
            return true;
        }
    }

    return false;
}

int Fw::Database::PostgreSQL::Query::columnInt(int column) const throw(const Fw::Exception&)
{
    if(PQgetisnull(m_result, m_currRow, column) == 0)
    {
        return atoi(PQgetvalue(m_result, m_currRow, column));
    }

    return 0;
}

uint Fw::Database::PostgreSQL::Query::columnUInt(int column) const throw(const Fw::Exception&)
{
    return columnText(column).toUInt();
}

double Fw::Database::PostgreSQL::Query::columnDouble(int column) const throw(const Fw::Exception&)
{
    return columnText(column).toDouble();
}

QString Fw::Database::PostgreSQL::Query::columnText(int column) const throw(const Fw::Exception&)
{
    const char* const result = PQgetvalue(m_result, m_currRow, column);
    return QString::fromUtf8(result);
}

FwColor Fw::Database::PostgreSQL::Query::columnColor(int column) const throw(const Fw::Exception&)
{
    return FwColor(columnInt(column));
}

QUrl Fw::Database::PostgreSQL::Query::columnUrl(int column) const throw(const Fw::Exception&)
{
    return QUrl(columnText(column));
}

QDateTime Fw::Database::PostgreSQL::Query::columnDateTime(int column) const throw(const Fw::Exception&)
{
    return QDateTime::fromTime_t(columnDouble(column));
}

QDate Fw::Database::PostgreSQL::Query::columnDate(int column) const throw(const Fw::Exception&)
{
    return QDate::fromString(columnText(column), Qt::ISODate);
}

QTime Fw::Database::PostgreSQL::Query::columnTime(int column) const throw(const Fw::Exception&)
{
    return QTime::fromString(columnText(column), Qt::ISODate);
}

QByteArray Fw::Database::PostgreSQL::Query::toUtf8() const throw(const Fw::Exception&)
{
    QByteArray query;
    for(Fw::Database::PostgreSQL::TokenVector::ConstIterator iter = m_tokens.begin(); iter != m_tokens.end(); ++iter)
    {
        if(iter->value.isEmpty())
        {
            throw Fw::Exception("Ones bind parameter is not initialized");
        }
        query += iter->value;
    }

    return query;
}
