#include <QtCore/qdebug.h>

#include <stdlib.h>

#include "fwpg.h"

#include "fwcore/fwml.h"
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

bool FwPg::QueryData::doExec() throw (Fw::Exception&)
{
    closeQuery(); //close query if exist

    Database* db = static_cast<Database*>(m_db);
    if(db == 0 || db->m_connection == 0)
    {
        throw Fw::Exception(static_cast<FwPg::Database*>(db)->lastError());
    }

    QByteArray query;
    for(FwPg::TokenVector::Iterator iter = m_tokens.begin(); iter != m_tokens.end(); ++iter)
    {
        if(iter->value.isEmpty())
        {
            throw Fw::Exception("Ones bind parameter is not initialized");
        }
        query += iter->value;
    }

    PGconn* connection = db->m_connection;

    //make query
    m_result = PQexec(connection, query.constData());
    ExecStatusType status = PQresultStatus(m_result);
    if (status == PGRES_FATAL_ERROR || status == PGRES_NONFATAL_ERROR)
    {
        closeQuery();
        throw Fw::Exception(static_cast<FwPg::Database*>(db)->lastError());
    }

    m_curr_row = 0;
    db->m_lastInsertRowId = 0;

    const Oid lastRow = PQoidValue(m_result);
    if(lastRow != InvalidOid)
    {
        db->m_lastInsertRowId = lastRow;
    }

    m_count_row = PQntuples(m_result);
    return m_count_row;
}

bool FwPg::QueryData::doNext() throw (Fw::Exception&)
{
    if(m_count_row == 0)
    {
        throw Fw::Exception("Row count is null");
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
    QByteArray value = "'" + text.toUtf8() + "'";
    for(FwPg::TokenVector::Iterator iter = m_tokens.begin(); iter != m_tokens.end(); ++iter)
    {
        QueryToken& token = *iter;
        if(token.param == index)
        {
            token.value = value;
        }
    }
}

void FwPg::QueryData::doBindDateTime(int index, const QDateTime& dateTime)
{
    doBindText(index, dateTime.toString(Qt::ISODate));
}

void FwPg::QueryData::doBindDate(int index, const QDate& date)
{
    doBindText(index, date.toString(Qt::ISODate));
}

void FwPg::QueryData::doBindTime(int index, const QTime& time)
{
    doBindText(index, time.toString(Qt::ISODate));
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

QDateTime FwPg::QueryData::doColumnDateTime(int column) const
{
    return QDateTime::fromString(doColumnText(column), Qt::ISODate);
}

QDate FwPg::QueryData::doColumnDate(int column) const
{
    return QDate::fromString(doColumnText(column), Qt::ISODate);
}
QTime FwPg::QueryData::doColumnTime(int column) const
{
    return QTime::fromString(doColumnText(column), Qt::ISODate);
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

FwPg::Database::Database(const QByteArray& name, QObject* parent) :
    BaseClass(name, parent),
    m_connection(0),
    m_lastInsertRowId(0)
{
}

FwPg::Database::~Database()
{
    close();
}

bool FwPg::Database::loadData(FwMLObject* object)
{
    FwMLString* hostNode = object->attribute("host")->cast<FwMLString>();
    if(hostNode && !hostNode->isEmpty())
    {
        setHost(hostNode->value());
    }

    bool bOk = false;
    int port = 0;
    FwMLNode* portNode = object->attribute("port");
    if(portNode && (port = portNode->toInt(&bOk)) && bOk)
    {
        setPort(port);
    }

    FwMLString* dbnameNode   = object->attribute("dbname")->cast<FwMLString>();
    if(dbnameNode && !dbnameNode->isEmpty())
    {
        setDbName(dbnameNode->value());
    }

    FwMLString* loginNode    = object->attribute("login")->cast<FwMLString>();
    if(loginNode && !loginNode->isEmpty())
    {
        setUser(loginNode->value());
    }

    FwMLString* passwordNode = object->attribute("password")->cast<FwMLString>();
    if(passwordNode && !passwordNode->isEmpty())
    {
        setPassword(passwordNode->value());
    }

    return true;
}

void FwPg::Database::resetData()
{
    m_conParams.clear();
}

void FwPg::Database::setHost(const QByteArray& host)
{
    m_conParams.host = host;
}

void FwPg::Database::setPort(int port)
{
    m_conParams.port = port;
}

void FwPg::Database::setDbName(const QByteArray& dbname)
{
    m_conParams.dbname = dbname;
}

void FwPg::Database::setUser(const QByteArray& user)
{
    m_conParams.user = user;
}

void FwPg::Database::setPassword(const QByteArray& password)
{
    m_conParams.password = password;
}

const QByteArray& FwPg::Database::getHost() const
{
    return m_conParams.host;
}

int FwPg::Database::getPort() const
{
    return m_conParams.port;
}

const QByteArray& FwPg::Database::getDbName() const
{
    return m_conParams.dbname;
}

const QByteArray& FwPg::Database::getUser() const
{
    return m_conParams.user;
}

const QByteArray& FwPg::Database::getPassword() const
{
    return m_conParams.password;
}

void FwPg::Database::init() throw(Fw::Exception&)
{
    //"hostaddr=0.0.0.0 port=0 dbname=str user=str password=str"
    QByteArray connection;
    connection += "hostaddr=" + m_conParams.host;
    connection += " port=" + QByteArray::number(m_conParams.port);
    connection += " dbname=" + m_conParams.dbname;
    connection += " user=" + m_conParams.user;
    connection += " password=" + m_conParams.password;

    m_connection = PQconnectdb(connection);
    if(!m_connection || PQstatus(m_connection) != CONNECTION_OK)
    {
        release();
        throw(Fw::Exception(lastError()));
    }
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

QString FwPg::Database::lastError() const
{
    if(this && m_connection)
    {
        return PQerrorMessage(m_connection);
    }

    return "No database connection";
}
