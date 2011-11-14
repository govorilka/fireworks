#include "fwcore/fwchartype.h"

#include "fw/database/postgresql/querydata.hpp"
#include "fw/database/postgresql/driver.hpp"

bool Fw::Database::PostgreSQL::parseQuery(const QByteArray& query, TokenVector& tokens)
{
    tokens.clear();
    if(query.isEmpty())
    {
        return false;
    }

    tokens.append(QueryToken());

    bool parserParamId = false;
    bool waitParam = false;
    for(QByteArray::const_iterator iter = query.begin(); iter != query.end(); ++iter)
    {
        switch(Fw::charType(iter))
        {
        case Fw::C_Que:
            if(waitParam)
            {
                tokens.last().value += *iter;
            }
            waitParam = !waitParam;
            break;

        case Fw::C_Num:
            if(waitParam)
            {
                tokens.append(QueryToken());
                parserParamId = true;
                waitParam = false;
            }
            tokens.last().value += (*iter);
            break;

        default:
            if(parserParamId)
            {
                parserParamId = false;
                tokens.last().swapParam();
                tokens.append(QueryToken());
            }
            else if(waitParam)
            {
                tokens.last().value += '?';
                waitParam = false;
            }
            tokens.last().value += *iter;
            break;
        }
    }

    if(parserParamId)
    {
        tokens.last().swapParam();
    }
    else if(waitParam)
    {
        tokens.last().value += '?';
    }

    return tokens.size();
}

////////////////////////////////////////////////////////////////////////////////

Fw::Database::PostgreSQL::QueryData::QueryData(Fw::Database::PostgreSQL::Driver* db, const Fw::Database::PostgreSQL::TokenVector& query) :
    BaseClass(db),
    m_result(0),
    m_tokens(query),
    m_countRow(0),
    m_currRow(0),
    m_lastInsertRowId(0)
{
}

Fw::Database::PostgreSQL::QueryData::~QueryData()
{
    closeQuery();
}

bool Fw::Database::PostgreSQL::QueryData::isNull() const
{
    return m_result == 0;
}

bool Fw::Database::PostgreSQL::QueryData::operator==(const QueryData& other) const
{
    return driver() == other.driver() && m_result == other.m_result;
}

bool Fw::Database::PostgreSQL::QueryData::operator!=(const QueryData& other) const
{
    return driver() != other.driver() && m_result != other.m_result;
}

bool Fw::Database::PostgreSQL::QueryData::doExec() throw (Fw::Exception&)
{
    closeQuery(); //close query if exist

    Driver* drv = static_cast<Driver*>(driver());
    if(drv == 0 || drv->m_connection == 0)
    {
        throw Fw::Exception(drv->lastError());
    }

    QByteArray query;
    for(Fw::Database::PostgreSQL::TokenVector::Iterator iter = m_tokens.begin(); iter != m_tokens.end(); ++iter)
    {
        if(iter->value.isEmpty())
        {
            throw Fw::Exception("Ones bind parameter is not initialized");
        }
        query += iter->value;
    }

    PGconn* connection = drv->m_connection;

    //make query
    m_result = PQexec(connection, query.constData());
    ExecStatusType status = PQresultStatus(m_result);
    if (status == PGRES_FATAL_ERROR || status == PGRES_NONFATAL_ERROR)
    {
        closeQuery();
        throw Fw::Exception(drv->lastError());
    }

    m_currRow = 0;
    drv->m_lastInsertRowId = 0;

    const Oid lastRow = PQoidValue(m_result);
    if(lastRow != InvalidOid)
    {
        drv->m_lastInsertRowId = lastRow;
    }

    m_countRow = PQntuples(m_result);
    return m_countRow;
}

bool Fw::Database::PostgreSQL::QueryData::doNext() throw (Fw::Exception&)
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

void Fw::Database::PostgreSQL::QueryData::doReset()
{
    m_currRow = 0;
}

void Fw::Database::PostgreSQL::QueryData::doFinalize()
{
    closeQuery();

    m_tokens.clear();
}

void Fw::Database::PostgreSQL::QueryData::doBindInt(int index, int value) throw(Fw::Exception&)
{
    for(Fw::Database::PostgreSQL::TokenVector::Iterator iter = m_tokens.begin(); iter != m_tokens.end(); ++iter)
    {
        QueryToken& token = *iter;
        if(token.param == index)
        {
            token.value.setNum(value);
        }
    }
}

void Fw::Database::PostgreSQL::QueryData::doBindText(int index, const QString& text) throw(Fw::Exception&)
{
    QByteArray value = "'" + text.toUtf8() + "'";
    for(Fw::Database::PostgreSQL::TokenVector::Iterator iter = m_tokens.begin(); iter != m_tokens.end(); ++iter)
    {
        QueryToken& token = *iter;
        if(token.param == index)
        {
            token.value = value;
        }
    }
}

void Fw::Database::PostgreSQL::QueryData::doBindDateTime(int index, const QDateTime& dateTime)
{
    doBindText(index, dateTime.toString(Qt::ISODate));
}

void Fw::Database::PostgreSQL::QueryData::doBindDate(int index, const QDate& date)
{
    doBindText(index, date.toString(Qt::ISODate));
}

void Fw::Database::PostgreSQL::QueryData::doBindTime(int index, const QTime& time)
{
    doBindText(index, time.toString(Qt::ISODate));
}

bool Fw::Database::PostgreSQL::QueryData::doColumnBool(int column) const
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

int Fw::Database::PostgreSQL::QueryData::doColumnInt(int column) const
{
    if(PQgetisnull(m_result, m_currRow, column) == 0)
    {
        return atoi(PQgetvalue(m_result, m_currRow, column));
    }

    return 0;
}

QString Fw::Database::PostgreSQL::QueryData::doColumnText(int column) const
{

    const char* const result = PQgetvalue(m_result, m_currRow, column);
    return QString(result);
}

FwColor Fw::Database::PostgreSQL::QueryData::doColumnColor(int column) const
{
    return FwColor(doColumnInt(column));
}

QUrl Fw::Database::PostgreSQL::QueryData::doColumnUrl(int column) const
{
    return QUrl(doColumnText(column));
}

QDateTime Fw::Database::PostgreSQL::QueryData::doColumnDateTime(int column) const
{
    return QDateTime::fromString(doColumnText(column), Qt::ISODate);
}

QDate Fw::Database::PostgreSQL::QueryData::doColumnDate(int column) const
{
    return QDate::fromString(doColumnText(column), Qt::ISODate);
}
QTime Fw::Database::PostgreSQL::QueryData::doColumnTime(int column) const
{
    return QTime::fromString(doColumnText(column), Qt::ISODate);
}

void Fw::Database::PostgreSQL::QueryData::closeQuery()
{
    if(m_result)
    {
        PQclear(m_result);
        m_result = 0;
    }
    m_countRow = m_currRow = 0;
}
