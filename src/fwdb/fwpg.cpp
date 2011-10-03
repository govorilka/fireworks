#include "fwpg.h"

#include <pgsql/libpq-fe.h>

namespace FwPg
{
    static char paramHost[] = "host";
    static char paramPort[] = "port";
    static char paramUser[] = "user";
    static char paramPassword[] = "password";
}

///////////////////////////////////////////////////////////////////////////////

FwPg::Exception::Exception(const QString& err) throw():
    BaseClass(),
    error(err)
{
}

FwPg::Exception::~Exception() throw()
{
}

const char* FwPg::Exception::what() const throw()
{
    return qPrintable(error);
}

///////////////////////////////////////////////////////////////////////////////

FwPg::ConnectionParams::ConnectionParams(const QByteArray& name) :
    BaseClass(name),
    port(0)
{
}

bool FwPg::ConnectionParams::loadData(FwMLObject* object)
{
return false;
}

QByteArray FwPg::ConnectionParams::toByteArray() const
{
    QByteArray connectionString;
    addParamToString(connectionString, paramHost, host);
    addParamToString(connectionString, paramPort, port);
    addParamToString(connectionString, paramUser, user);
    addParamToString(connectionString, paramPassword, password);
    return connectionString;
}

void FwPg::ConnectionParams::addParamToString(QByteArray& result, const QByteArray& param, const QByteArray& value)
{
    if(!value.isEmpty())
    {
        if(!result.isEmpty())
        {
            result += " ";
        }
        result = result + param + "=" + value;
    }
}

void FwPg::ConnectionParams::addParamToString(QByteArray& result, const QByteArray& param, int value)
{
    if(value)
    {
        if(!result.isEmpty())
        {
            result += " ";
        }
        result = result + param + "=" + QByteArray::number(value);
    }
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

void FwPg::Database::open(const QByteArray& connectionString) throw (Exception&)
{
    close();

    m_connection = PQconnectdb(connectionString);

    if(!m_connection)
        throw(Exception("Connection fail!"));

    if (PQstatus(m_connection) != CONNECTION_OK)
    {
        QString l_error("Connection fail : ");
        l_error += PQerrorMessage(m_connection);

        close();

        throw(Exception(l_error));
    }
}

void FwPg::Database::close()
{
    if(m_connection)
    {
        PQfinish(m_connection);
        m_connection = 0;
    }
}
