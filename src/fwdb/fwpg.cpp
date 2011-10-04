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
    bool parserParamId = true;

    for(QByteArray::const_iterator iter = query.begin(); iter != query.end(); ++iter)
    {
        switch(Fw::charType(iter))
        {
        case Fw::C_Que:
            parserParamId = true;
            break;

        case Fw::C_Num:
            break;

        default:
            break;
        }
    }
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

FwPg::QueryData::QueryData(Database* db, const QByteArray& query) :
    BaseClass(db),
    m_result(0)
{
    FwPg::parseQuery(query, m_tokens);
}

FwPg::QueryData::~QueryData()
{
    release();
}

void FwPg::QueryData::finalize()
{
    if(m_result)
    {
        PQclear(m_result);
        m_result = 0;
    }
    m_tokens.clear();
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
    return new QueryData(this, query.toUtf8());
}
