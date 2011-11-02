#ifndef FIREWORKS_DB_H
#define FIREWORKS_DB_H

#include <QtCore/qobject.h>
#include <QtCore/qsharedpointer.h>
#include <QtCore/qreadwritelock.h>
#include <QtCore/qurl.h>
#include <QtCore/qdatetime.h>

#include "fireworks.h"
#include "fwcore/fwcppobject.h"
#include "fwcore/fwcolor.h"

namespace Fw
{
    class Exception;
    class Query;
    class QueryData;
    class Database;
    class DatabaseLocker;
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

    inline QByteArray error() const;

protected:
    QByteArray m_error;
};

QByteArray Fw::Exception::error() const
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

    virtual bool doExec() throw (Fw::Exception&) = 0;
    virtual bool doNext() throw (Fw::Exception&) = 0;
    virtual void doReset() = 0;
    virtual void doFinalize() = 0;

    virtual void doBindInt(int index, int value) throw(Exception&) = 0;
    virtual void doBindText(int index, const QString& text) throw(Exception&) = 0;
    virtual void doBindDateTime(int index, const QDateTime& dateTime) = 0;
    virtual void doBindDate(int index, const QDate& date) = 0;
    virtual void doBindTime(int index, const QTime& time) = 0;

    virtual bool doColumnBool(int column) const = 0;
    virtual int doColumnInt(int column) const = 0;
    virtual QString doColumnText(int column) const = 0;
    virtual FwColor doColumnColor(int column) const = 0;
    virtual QUrl doColumnUrl(int column) const = 0;
    virtual QDateTime doColumnDateTime(int column) const = 0;
    virtual QDate doColumnDate(int column) const = 0;
    virtual QTime doColumnTime(int column) const = 0;

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

class FIREWORKSSHARED_EXPORT Fw::Database : public QObject, public FwCPPObject
{
    Q_OBJECT
    typedef FwCPPObject BaseClass;

    friend class QueryData;
    friend class DatabaseLocker;

public:
    explicit Database(const QByteArray& name, QObject* parent = 0);
    virtual ~Database();

    void open() throw(Exception&);
    void close() throw();
    inline bool isOpen() const;

    Query query(const QString& query) throw(Exception&);

    void beginTransaction() throw(Exception&);
    void commit() throw(Exception&);
    void rollback() throw(Exception&);

    virtual int lastInsertKey() = 0;

    void reindex(const QString& indexName) throw(Exception&);
    void execFile(const QString& fileName) throw(Exception&);
    void execFile(QIODevice* device) throw(Exception&);

protected:
    virtual void init() throw(Exception&) = 0;
    virtual void release() throw() = 0;

    virtual QueryData* createQuery(const QString& query) throw(Exception&) = 0;

private:
    bool m_open;
    bool m_begin_transaction;
    QList<QueryData*> m_queries;
    QReadWriteLock m_dbLock;
};

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

#endif // FIREWORKS_DB_H

#include "fwdb_inl.hpp"
