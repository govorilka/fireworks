#ifndef FIREWORKS_PG_H
#define FIREWORKS_PG_H

#include <QtCore/qobject.h>

#include <pgsql/libpq-fe.h>

#include "fireworks.h"

#include "fwcore/fwcppobject.h"

namespace FwPg
{
   class Exception;
   class ConnectionParams;
   class QueryData;
   class Query;
   class Database;
}

class FwPg::Exception: public std::exception
{
    typedef std::exception BaseClass;

public:
    Exception(const QString& err) throw();
    virtual ~Exception() throw();

    virtual const char* what() const throw();

    const QString error;
};

///////////////////////////////////////////////////////////////////////////////

class FwPg::ConnectionParams : public FwCPPObject
{
    typedef FwCPPObject BaseClass;

public:
    QByteArray host;
    short int  port;
    QByteArray database;
    QByteArray user;
    QByteArray password;

    ConnectionParams(const QByteArray& name = "connection");

    virtual bool loadData(FwMLObject* object);

    QByteArray toByteArray() const;

private:
    static void addParamToString(QByteArray& result, const QByteArray& param, const QByteArray& value);
    static void addParamToString(QByteArray& result, const QByteArray& param, int value);
};

////////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT QueryData
{
public:
    friend class Database;

    ~QueryData();

    inline Database* parent() const;
    inline PGresult* result() const;

    void clear();

protected:
    QueryData(Database* parent);

private:
    Database* m_parent;
    PGresult* m_result;
};

Database* QueryData::parent() const
{
    return m_parent;
}

PGresult* QueryData::result() const
{
    return m_result;
}

////////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT Query : protected QSharedPointer<QueryData>
{
    typedef QSharedPointer<QueryData> BaseClass;

public:
    friend class Database;

    ~Query();

protected:
    Query(QueryData * data);
};

///////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT FwPg::Database : public QObject
{
    Q_OBJECT
    typedef QObject BaseClass;

public:
    Database(QObject* parent = 0);
    virtual ~Database();

    inline bool isOpen() const;

    inline void open(const ConnectionParams& params) throw (Exception&);
    void open(const QByteArray& connectionString) throw (Exception&);

    void close();

private:
    PGconn* m_connection;

};

///////////////////////////////////////////////////////////////////////////////

void FwPg::Database::open(const ConnectionParams& params) throw (Exception&)
{
    open(params.toByteArray());
}

bool FwPg::Database::isOpen() const
{
    return m_connection;
}

#endif //FIREWORKS_PG_H
