#include <QtCore/qdebug.h>

#include "fwpg.h"

#include "fwcore/fwchartype.h"

//namespace FwPg
//{
//    static char paramHost[] = "host";
//    static char paramPort[] = "port";
//    static char paramUser[] = "user";
//    static char paramPassword[] = "password";
//}

///////////////////////////////////////////////////////////////////////////////

//FwPg::ConnectionParams::ConnectionParams(const QByteArray& name) :
//    BaseClass(name),
//    port(0)
//{
//}

//bool FwPg::ConnectionParams::loadData(FwMLObject* object)
//{
//return false;
//}

//QByteArray FwPg::ConnectionParams::toByteArray() const
//{
//    QByteArray connectionString;
//    addParamToString(connectionString, paramHost, host);
//    addParamToString(connectionString, paramPort, port);
//    addParamToString(connectionString, paramUser, user);
//    addParamToString(connectionString, paramPassword, password);
//    return connectionString;
//}

//void FwPg::ConnectionParams::addParamToString(QByteArray& result, const QByteArray& param, const QByteArray& value)
//{
//    if(!value.isEmpty())
//    {
//        if(!result.isEmpty())
//        {
//            result += " ";
//        }
//        result = result + param + "=" + value;
//    }
//}

//void FwPg::ConnectionParams::addParamToString(QByteArray& result, const QByteArray& param, int value)
//{
//    if(value)
//    {
//        if(!result.isEmpty())
//        {
//            result += " ";
//        }
//        result = result + param + "=" + QByteArray::number(value);
//    }
//}

///////////////////////////////////////////////////////////////////////////////

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
    m_tokens(query)
{
}

FwPg::QueryData::~QueryData()
{
    release();
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
    //TODO
    throw Exception("TODO");
}

bool FwPg::QueryData::doNext() throw (Fw::Exception&)
{
    //TODO
    throw Exception("TODO");
}
void FwPg::QueryData::doReset()
{
    //TODO
}

void FwPg::QueryData::doFinalize()
{
    if(m_result)
    {
        PQclear(m_result);
        m_result = 0;
    }
    m_tokens.clear();
}

bool FwPg::QueryData::columnBool(int column)
{
    //TODO
    return false;
}

int FwPg::QueryData::columnInt(int column)
{
    //TODO
    return 0;
}

QString FwPg::QueryData::columnText(int column)
{
    //TODO
    return QString();
}

FwColor FwPg::QueryData::columnColor(int column)
{
    //TODO
    return FwColor();
}

QUrl FwPg::QueryData::columnUrl(int column)
{
    //TODO
    return QUrl();
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
    return false;
}

int FwPg::QueryData::doColumnInt(int column) const
{
   return 0;
}

QString FwPg::QueryData::doColumnText(int column) const
{
    return QString();
}

FwColor FwPg::QueryData::doColumnColor(int column) const
{
    return FwColor();
}
QUrl FwPg::QueryData::doColumnUrl(int column) const
{
    return QUrl();
}

void FwPg::QueryData::exec() throw (Fw::Exception&)
{
    if(m_result)
    {
        PQclear(m_result);
        m_result = 0;
    }

    QByteArray query;
    for(FwPg::TokenVector::Iterator iter = m_tokens.begin(); iter != m_tokens.end(); ++iter)
    {
        if(!iter->value.isEmpty())
        {
            throw FwPg::Exception("Ones bind parameter is not initialized");
        }
        query += iter->value;
    }

    //TODO:
}

///////////////////////////////////////////////////////////////////////////////

FwPg::Database::Database(QObject* parent) :
    BaseClass(parent),
    m_connection(0)
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
    }
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
