#include <QtCore/qdebug.h>

#include <stdlib.h>

#include "fwpg.h"

#include "fwcore/fwchartype.h"

///////////////////////////////////////////////////////////////////////////////
namespace
{

//void swapBytes(char& a, char& b)
//{
//    a = a ^ b;
//    b = b ^ a;
//    a = a ^ b;
//}

//void reverceBytes(char* arr, int size)
//{
//    char* begin = arr;
//    char* end = arr+size-1;

//    while(begin < end)
//    {
//        swapBytes(*begin, *end);
//        ++begin;
//        --end;
//    }
//}

}
//namespace

bool FwPg::parseQuery(const QByteArray& query, TokenVector& tokens)
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

///////////////////////////////////////////////////////////////////////////////

FwPg::Exception::Exception(const Database* db) throw() :
    BaseClass(db)
{
    if(db && db->m_connection)
    {
        m_error = PQerrorMessage(db->m_connection);
    }
    else
    {
        m_error = "No database connection";
    }
}

FwPg::Exception::Exception(const QString& error) throw() :
    BaseClass(error)
{
}

FwPg::Exception::~Exception() throw()
{
}

////////////////////////////////////////////////////////////////////////////////

FwPg::QueryData::QueryData(Database* db, const TokenVector& query) :
    BaseClass(db),
    m_result(0),
    m_tokens(query),
    m_count_row(0),
    m_curr_row(0),
    m_lastInsertRowId(0)
{
}

FwPg::QueryData::~QueryData()
{
    closeQuery();
}

bool FwPg::QueryData::isNull() const
{
    return m_result == 0;
}

bool FwPg::QueryData::operator==(const QueryData& other) const
{
    return m_db == other.m_db && m_result == other.m_result;
}

bool FwPg::QueryData::operator!=(const QueryData& other) const
{
    return m_db != other.m_db && m_result != other.m_result;
}

void FwPg::QueryData::doExec() throw (Fw::Exception&)
{
    closeQuery(); //close query if exist

    Database* db = dynamic_cast<Database*>(m_db);
    if(db == 0 || db->m_connection == 0)
    {
        throw Exception(db);
    }

    QByteArray query;
    for(FwPg::TokenVector::Iterator iter = m_tokens.begin(); iter != m_tokens.end(); ++iter)
    {
        if(iter->value.isEmpty())
        {
            throw Exception("Ones bind parameter is not initialized");
        }
        query += iter->value;
    }

    PGconn* connection = db->m_connection;

    //make query
    m_result = PQexec(connection, query.constData());
    ExecStatusType status = PQresultStatus(m_result);
    if (status == PGRES_FATAL_ERROR || status == PGRES_FATAL_ERROR)
    {
        closeQuery();
        throw Exception(db);
    }

    m_count_row = PQntuples(m_result);
    m_curr_row = 0;

    const Oid lastRow = PQoidValue(m_result);
    if(lastRow != InvalidOid)
    {
        db->m_lastInsertRowId = lastRow;
    }
}

bool FwPg::QueryData::doNext() throw (Fw::Exception&)
{
    if(m_count_row == 0)
    {
        throw Exception("Row count is null");
    }

    ++m_curr_row;
    if(m_curr_row < m_count_row)
    {
        return true;
    }
    else
    {
        doReset();
        return false;
    }
}

void FwPg::QueryData::doReset()
{
    m_curr_row = 0;
}

void FwPg::QueryData::doFinalize()
{
    closeQuery();

    m_tokens.clear();
}

void FwPg::QueryData::doBindInt(int index, int value) throw(Fw::Exception&)
{
    for(FwPg::TokenVector::Iterator iter = m_tokens.begin(); iter != m_tokens.end(); ++iter)
    {
        QueryToken& token = *iter;
        if(token.param == index)
        {
            token.value.setNum(value);
        }
    }
}

void FwPg::QueryData::doBindText(int index, const QString& text) throw(Fw::Exception&)
{
    for(FwPg::TokenVector::Iterator iter = m_tokens.begin(); iter != m_tokens.end(); ++iter)
    {
        QueryToken& token = *iter;
        if(token.param == index)
        {
            token.value = text.toUtf8();
        }
    }
}

void FwPg::QueryData::doBindDateTime(int index, const QDateTime& dateTime)
{
    QByteArray dateStr = dateTime.toString(Qt::ISODate).toUtf8();
    for(FwPg::TokenVector::Iterator iter = m_tokens.begin(); iter != m_tokens.end(); ++iter)
    {
        QueryToken& token = *iter;
        if(token.param == index)
        {
            token.value = dateStr;
        }
    }
}

bool FwPg::QueryData::doColumnBool(int column) const
{
    if(PQgetisnull(m_result, m_curr_row, column) == 0)
    {
        if('t' == *PQgetvalue(m_result, m_curr_row, column))
        {
            return true;
        }
    }

    return false;
}

int FwPg::QueryData::doColumnInt(int column) const
{
    if(PQgetisnull(m_result, m_curr_row, column) == 0)
    {
        return atoi(PQgetvalue(m_result, m_curr_row, column));
    }

    return 0;
}

QString FwPg::QueryData::doColumnText(int column) const
{

    const char* const result = PQgetvalue(m_result, m_curr_row, column);
    return QString(result);
}

FwColor FwPg::QueryData::doColumnColor(int column) const
{
    return FwColor(doColumnInt(column));
}

QUrl FwPg::QueryData::doColumnUrl(int column) const
{
    return QUrl(doColumnText(column));
}

void FwPg::QueryData::closeQuery()
{
    if(m_result)
    {
        PQclear(m_result);
        m_result = 0;
    }
    m_count_row = m_curr_row = 0;
}

///////////////////////////////////////////////////////////////////////////////

FwPg::Database::Database(QObject* parent) :
    BaseClass(parent),
    m_connection(0),
    m_lastInsertRowId(0)
{
}

FwPg::Database::~Database()
{
    close();
}

bool FwPg::Database::init(const QString& param) throw(Fw::Exception&)
{
    m_connection = PQconnectdb(param.toUtf8());
    if(m_connection && PQstatus(m_connection) == CONNECTION_OK)
    {
        return true;
    }
    release();
    throw(Exception(this));
    return false;
}

void FwPg::Database::release() throw()
{
    if(m_connection)
    {
        PQfinish(m_connection);
        m_connection = 0;
        m_lastInsertRowId = 0;
    }
}

int FwPg::Database::lastInsertKey()
{
    return m_lastInsertRowId;
}

FwPg::QueryData* FwPg::Database::createQuery(const QString& query) throw(Fw::Exception&)
{
    TokenVector vector;
    if(FwPg::parseQuery(query.toUtf8(), vector))
    {
        return new QueryData(this, vector);
    }
    return 0;
}
