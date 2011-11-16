#include "fwcore/fwchartype.h"
#include "fwcore/fwml.h"

#include "fw/database/postgresql/driver.hpp"
#include "fw/database/postgresql/querydata.hpp"

namespace
{
bool parseQuery(const QByteArray& query, Fw::Database::PostgreSQL::TokenVector& tokens)
{
    typedef Fw::Database::PostgreSQL::QueryToken QueryToken;

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
}

struct Fw::Database::PostgreSQL::Driver::ConnectionParams
{
    QByteArray host;
    int        port;

    QByteArray dbname;

    QByteArray user;
    QByteArray password;

    ConnectionParams() : port(0){};
    void clear()
    {
        host.clear();
        port = 0;
        dbname.clear();
        user.clear();
        password.clear();
    }
};

QString Fw::Database::PostgreSQL::Driver::lastError() const
{
    if(this && m_connection)
    {
        return PQerrorMessage(m_connection);
    }
    return BaseClass::lastError();
}

Fw::Database::PostgreSQL::Driver::Driver(const QByteArray& name) :
    BaseClass(name),
    m_connection(0),
    m_lastInsertRowId(0),
    m_conParams(0)
{
    m_conParams = new ConnectionParams();
}

Fw::Database::PostgreSQL::Driver::~Driver()
{
    close();

    delete m_conParams;
}

bool Fw::Database::PostgreSQL::Driver::loadData(FwMLObject* object) throw(const Fw::Exception&)
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

void Fw::Database::PostgreSQL::Driver::resetData()
{
    m_conParams->clear();
}

void Fw::Database::PostgreSQL::Driver::open() throw(const Fw::Exception&)
{
    //"hostaddr=0.0.0.0 port=0 dbname=str user=str password=str"
    QByteArray connection;
    connection += "hostaddr=" + m_conParams->host;
    connection += " port=" + QByteArray::number(m_conParams->port);
    connection += " dbname=" + m_conParams->dbname;
    connection += " user=" + m_conParams->user;
    connection += " password=" + m_conParams->password;

    m_connection = PQconnectdb(connection);
    if(!m_connection || PQstatus(m_connection) != CONNECTION_OK)
    {
        close();
        throw(Fw::Exception(lastError()));
    }
}

void Fw::Database::PostgreSQL::Driver::close() throw()
{
    if(m_connection)
    {
        PQfinish(m_connection);
        m_connection = 0;
        m_lastInsertRowId = 0;
    }
}

Fw::Database::QueryPtr Fw::Database::PostgreSQL::Driver::createQuery(const DriverPtr& driver, const QString& query) throw(const Fw::Exception&)
{
    TokenVector vector;
    if(!parseQuery(query.toUtf8(), vector))
    {
        throw Fw::Exception("Can't parse query");
    }
    return QueryPtr(new Query(driver, vector));
}

bool Fw::Database::PostgreSQL::Driver::isOpen() const
{
    return m_connection;
}
int Fw::Database::PostgreSQL::Driver::lastInsertKey() const
{
    return m_lastInsertRowId;
}

void Fw::Database::PostgreSQL::Driver::setLastInsertKey(int key)
{
    m_lastInsertRowId = key;
}

const PGconn* Fw::Database::PostgreSQL::Driver::connection() const
{
    return m_connection;
}

PGconn* Fw::Database::PostgreSQL::Driver::connection()
{
    return m_connection;
}

void Fw::Database::PostgreSQL::Driver::setHost(const QByteArray& host)
{
    m_conParams->host = host;
}

void Fw::Database::PostgreSQL::Driver::setPort(int port)
{
    m_conParams->port = port;
}

void Fw::Database::PostgreSQL::Driver::setDbName(const QByteArray& dbname)
{
    m_conParams->dbname = dbname;
}

void Fw::Database::PostgreSQL::Driver::setUser(const QByteArray& user)
{
    m_conParams->user = user;
}

void Fw::Database::PostgreSQL::Driver::setPassword(const QByteArray& password)
{
    m_conParams->password = password;
}

const QByteArray& Fw::Database::PostgreSQL::Driver::getHost() const
{
    return m_conParams->host;
}

int Fw::Database::PostgreSQL::Driver::getPort() const
{
    return m_conParams->port;
}

const QByteArray& Fw::Database::PostgreSQL::Driver::getDbName() const
{
    return m_conParams->dbname;
}

const QByteArray& Fw::Database::PostgreSQL::Driver::getUser() const
{
    return m_conParams->user;
}

const QByteArray& Fw::Database::PostgreSQL::Driver::getPassword() const
{
    return m_conParams->password;
}
