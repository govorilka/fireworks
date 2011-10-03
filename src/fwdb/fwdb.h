#ifndef FIREWORKS_DB_H
#define FIREWORKS_DB_H

#include <QtCore/qobject.h>
#include <QtCore/qsharedpointer.h>

#include <pgsql/libpq-fe.h>

#include "fireworks.h"

#include "fwcore/fwcppobject.h"

namespace Fw
{
    class Exception;
    class Query;
    class QueryData;
    class Database;

    Database* dbFactory(QObject* parent, const QString& driver, const QString& params);
}

///////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT Fw::Exception: public std::exception
{
    typedef std::exception BaseClass;

public:
    Exception(const Database* db) throw();
    virtual ~Exception() throw();

    virtual const char* what() const throw();

    inline const QString error() const;

protected:
    QString m_error;
};
\
const QString Fw::Exception::error() const
{
    return m_error;
}

///////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT Fw::Query : protected QSharedPointer<Fw::QueryData>
{
    typedef QSharedPointer<Fw::QueryData> BaseClass;

public:
    friend class Database;

    Query();
    ~Query();

protected:
    Query(QueryData* data);
};

////////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT Fw::QueryData
{
public:
    virtual ~QueryData();

    void release();
    virtual void finalize() = 0;

    inline Database* database() const;

protected:
    QueryData(Database* db);

    Database* m_db;

};

Fw::Database* Fw::QueryData::database() const
{
    return m_db;
}
///////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT Fw::Database : public QObject
{
    Q_OBJECT
    typedef QObject BaseClass;

    friend class QueryData;

public:
    Database(QObject* parent);
    virtual ~Database();

    Query query(const QString& query) throw(Exception&);

    void open(const QString& param) throw(Exception&);
    void close() throw();

    inline bool isOpen() const;

protected:
    virtual bool init(const QString& param) throw(Exception&) = 0;
    virtual void release() throw() = 0;

    virtual QueryData* createQuery(const QString& query) const throw(Exception&) = 0;
    \
private:
    bool m_open;
    QList<QueryData*> m_queries;
};

bool Fw::Database::isOpen() const
{
    return m_open;
}

#endif // FIREWORKS_DB_H
