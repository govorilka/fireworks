#ifndef FIREWORKS_PG_H
#define FIREWORKS_PG_H

#include "fwdb.h"

namespace FwPg
{
    class Exception;
    //class ConnectionParams;
    class QueryData;
    class Database;
}

/////////////////////////////////////////////////////////////////////////////////

//class FwPg::ConnectionParams : public FwCPPObject
//{
//    typedef FwCPPObject BaseClass;

//public:
//    QByteArray host;
//    short int  port;
//    QByteArray database;
//    QByteArray user;
//    QByteArray password;

//    ConnectionParams(const QByteArray& name = "connection");

//    virtual bool loadData(FwMLObject* object);

//    QByteArray toByteArray() const;

//private:
//    static void addParamToString(QByteArray& result, const QByteArray& param, const QByteArray& value);
//    static void addParamToString(QByteArray& result, const QByteArray& param, int value);
//};

///////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT FwPg::Exception: public Fw::Exception
{
    typedef Fw::Exception BaseClass;

public:
    Exception(const Database* db) throw();
    Exception(const QString& error) throw();
    virtual ~Exception() throw();
};

//////////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT FwPg::QueryData : public Fw::QueryData
{
    typedef Fw::QueryData BaseClass;
    friend class FwPg::Database;

public:
    ~QueryData();

    void finalize();

    inline PGresult* result() const;

protected:
    QueryData(Database* db, const QByteArray& query);

private:
    PGresult* m_result;
    QByteArray m_query;
};

PGresult* FwPg::QueryData::result() const
{
    return m_result;
}

//////////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT FwPg::Database : public Fw::Database
{
    Q_OBJECT
    typedef Fw::Database BaseClass;

    friend class Exception;

public:
    Database(QObject* parent = 0);
    virtual ~Database();

protected:
    virtual bool init(const QString& param) throw(Fw::Exception&);
    virtual void release() throw();

    virtual QueryData* createQuery(const QString& query) throw(Fw::Exception&);

private:
    PGconn* m_connection;

};

/////////////////////////////////////////////////////////////////////////////////

//void FwPg::Database::open(const ConnectionParams& params) throw (Exception&)
//{
//    open(params.toByteArray());
//}

//bool FwPg::Database::isOpen() const
//{
//    return m_connection;
//}

#endif //FIREWORKS_PG_H
