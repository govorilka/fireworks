#ifndef FIREWORKS_DB_H
#define FIREWORKS_DB_H

#include <QtCore/qobject.h>
#include <QtCore/qsharedpointer.h>
#include <QtCore/qreadwritelock.h>
#include <QtCore/qurl.h>

#ifdef FW_SUPPORT_POSTGRESQL
#include <pgsql/libpq-fe.h>
#endif

#include "fireworks.h"

#include "fwcore/fwcolor.h"

#include "fwcore/fwcppobject.h"

namespace Fw
{
    class Exception;
    class Query;
    class QueryData;
    class Database;
    class DatabaseLocker;

    Database* dbFactory(QObject* parent, const QString& driver, const QString& params);
}

///////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT Fw::Exception: public std::exception
{
    typedef std::exception BaseClass;

public:
    Exception(const Database* db) throw();
    Exception(const QString& error) throw();
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

    class iterator;

    Query();
    ~Query();

    void reset();
    bool exec() throw (Fw::Exception&);
    void finalize();

    void bindInt(int index, int value);
    void bindText(int index, const QString& text);
    void bindColor(int index, const FwColor& color);
    inline void bindUrl(int index, const QUrl& url);
    void bindDateTime(int index, const QDateTime& datetime);

    iterator begin();
    iterator end();

protected:
    Query(QueryData* data);
};

void Fw::Query::bindUrl(int index, const QUrl& url)
{
    //TODO
}


class Fw::Query::iterator
{
public:
    iterator(QueryData* data);
    iterator& operator ++();
    bool operator !=(const iterator& rhs);

    bool columnBool(int column);
    int columnInt(int column);
    QString columnText(int column);
    FwColor columnColor(int column);
    QUrl columnUrl(int column);

private:
    QueryData* m_data;
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
    friend class DatabaseLocker;

public:
    Database(QObject* parent);
    virtual ~Database();

    Query query(const QString& query) throw(Exception&);

    void execFile(const QString& fileName) throw(Fw::Exception&);
    void execFile(QIODevice* device) throw(Fw::Exception&);

    void open(const QString& param) throw(Exception&);
    void close() throw();

    inline bool isOpen() const;

    void rollback();
    void beginTransaction();
    void commit();

    int lastInsertKey();
    void reindex(const QString& indexName) throw(Fw::Exception&);

protected:
    virtual bool init(const QString& param) throw(Exception&) = 0;
    virtual void release() throw() = 0;

    virtual QueryData* createQuery(const QString& query) throw(Exception&) = 0;

private:
    bool m_open;
    QList<QueryData*> m_queries;
    QReadWriteLock m_dbLock;
};

bool Fw::Database::isOpen() const
{
    return m_open;
}

///////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT Fw::DatabaseLocker
{
public:
    DatabaseLocker(Database* db);
    ~DatabaseLocker();

    inline Database* db() const;

    bool lock() const;
    bool tryLock() const;

    void unlock();

private:
    mutable bool m_lock;
    Database* m_db;
};

Fw::Database* Fw::DatabaseLocker::db() const
{
    return m_db;
}

#endif // FIREWORKS_DB_H
