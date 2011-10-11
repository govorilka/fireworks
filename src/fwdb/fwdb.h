#ifndef FIREWORKS_DB_H
#define FIREWORKS_DB_H

#include <QtCore/qobject.h>
#include <QtCore/qsharedpointer.h>
#include <QtCore/qreadwritelock.h>
#include <QtCore/qurl.h>
#include <QtCore/qdatetime.h>

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

const QString Fw::Exception::error() const
{
    return m_error;
}

////////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT Fw::QueryData
{
public:
    friend class Fw::Query;

    virtual ~QueryData();

    void release();

    inline Database* database() const;
    inline bool isExec() const;

protected:
    QueryData(Database* db);

    bool step() throw (Fw::Exception&);
    void reset();

    virtual void doExec() throw (Fw::Exception&) = 0;
    virtual bool doNext() throw (Fw::Exception&) = 0;
    virtual void doReset() = 0;
    virtual void doFinalize() = 0;

    virtual void doBindInt(int index, int value) throw(Exception&) = 0;
    virtual void doBindText(int index, const QString& text) throw(Exception&) = 0;
    virtual void doBindDateTime(int index, const QDateTime& dateTime) = 0;

    virtual bool doColumnBool(int column) const = 0;
    virtual int doColumnInt(int column) const = 0;
    virtual QString doColumnText(int column) const = 0;
    virtual FwColor doColumnColor(int column) const = 0;
    virtual QUrl doColumnUrl(int column) const = 0;

    Database* m_db;

private:
    bool m_exec;
};

Fw::Database* Fw::QueryData::database() const
{
    return m_db;
}

bool Fw::QueryData::isExec() const
{
    return m_exec;
}

///////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT Fw::Query : protected QSharedPointer<Fw::QueryData>
{
    typedef QSharedPointer<Fw::QueryData> BaseClass;

public:
    friend class Database;

    Query();
    ~Query();

    inline bool step() throw (Fw::Exception&);
    inline void reset();

    inline void bindInt(int index, int value) throw(Exception&);
    inline void bindText(int index, const QString& text) throw(Exception&);
    inline void bindColor(int index, const FwColor& color) throw(Exception&);
    inline void bindUrl(int index, const QUrl& url);
    inline void bindDateTime(int index, const QDateTime& datetime) throw(Exception&);

    inline bool columnBool(int column) const;
    inline int columnInt(int column) const;
    inline QString columnText(int column) const;
    inline FwColor columnColor(int column) const;
    inline QUrl columnUrl(int column) const;


protected:
    Query(QueryData* data);

    static void doDeleteQueryData(QueryData* data);

    QueryData* getQueryData() const throw (Fw::Exception&);
    QueryData* getBindQueryData() const throw (Fw::Exception&);
};

bool Fw::Query::step() throw (Fw::Exception&)
{
    return getQueryData()->step();
}

void Fw::Query::reset()
{
    getQueryData()->reset();
}

void Fw::Query::bindInt(int index, int value) throw(Exception&)
{
    getBindQueryData()->doBindInt(index, value);
}

void Fw::Query::bindText(int index, const QString& text) throw(Exception&)
{
    getBindQueryData()->doBindText(index, text);
}

void Fw::Query::bindColor(int index, const FwColor& color) throw(Exception&)
{
    getBindQueryData()->doBindInt(index, static_cast<qint32>(color.argb()));
}

void Fw::Query::bindUrl(int index, const QUrl& url)
{
    getBindQueryData()->doBindText(index, url.toString());
}

void Fw::Query::bindDateTime(int index, const QDateTime& datetime) throw(Exception&)
{
    getBindQueryData()->doBindDateTime(index, datetime);
}

bool Fw::Query::columnBool(int column) const
{
    return getQueryData()->doColumnBool(column);
}

int Fw::Query::columnInt(int column) const
{
    return getQueryData()->doColumnInt(column);
}

QString Fw::Query::columnText(int column) const
{
    return getQueryData()->doColumnText(column);
}

FwColor Fw::Query::columnColor(int column) const
{
    return getQueryData()->doColumnColor(column);
}

QUrl Fw::Query::columnUrl(int column) const
{
    return getQueryData()->doColumnUrl(column);
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

    void open(const QString& param) throw(Exception&);
    void close() throw();
    inline bool isOpen() const;

    Query query(const QString& query) throw(Exception&);

    void beginTransaction() throw(Exception&);
    void commit() throw(Exception&);
    void rollback() throw(Exception&);

    int lastInsertKey();
    void reindex(const QString& indexName) throw(Exception&);
    void execFile(const QString& fileName) throw(Exception&);
    void execFile(QIODevice* device) throw(Exception&);

protected:
    virtual bool init(const QString& param) throw(Exception&) = 0;
    virtual void release() throw() = 0;

    virtual QueryData* createQuery(const QString& query) throw(Exception&) = 0;

private:
    bool m_open;
    bool m_begin_transaction;
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
    Database* m_db;
    mutable bool m_lock;
};

Fw::Database* Fw::DatabaseLocker::db() const
{
    return m_db;
}

#endif // FIREWORKS_DB_H
