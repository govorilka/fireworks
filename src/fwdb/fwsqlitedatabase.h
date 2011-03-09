#ifndef FIREWORKS_SQLITEDATABASE_H
#define FIREWORKS_SQLITEDATABASE_H

#include <QtCore/qobject.h>

#include <QtCore/qobject.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qreadwritelock.h>

#include "fwdb/sqlite/sqlite3.h"

#include "fwdb/fwsqlitequery.h"

class FwSQLiteDatabase : public QObject
{
    Q_OBJECT
    typedef QObject BaseClass;

public:
    friend class FwSQLiteQueryData;
    friend class FwSQLiteDBLock;

    FwSQLiteDatabase(QObject* parent = 0);
    virtual ~FwSQLiteDatabase();

    void open(const QString& fileName) throw(FwSQLiteException&);
    void open(const QString& fileName, int flags) throw(FwSQLiteException&);

    void close();

    FwSQLiteQuery query(const QString& query) throw(FwSQLiteException&);

    void exec(const QString& query) throw(FwSQLiteException&);

    void execFile(const QString& fileName) throw(FwSQLiteException&);
    void execFile(QIODevice* device) throw(FwSQLiteException&);

    void beginTransaction() throw(FwSQLiteException&);
    void commit() throw(FwSQLiteException&);
    void rollback() throw(FwSQLiteException&);

    inline bool isTransactionBegin() const;

    int lastInsertKey() const;

    void reindex(const QString& indexName) throw(FwSQLiteException&);

private:
    sqlite3 *m_db;
    bool m_beginTransaction;
    QList<FwSQLiteQueryData*> queries;
    QReadWriteLock m_dbLock;
};

bool FwSQLiteDatabase::isTransactionBegin() const
{
    return m_beginTransaction;
}

//////////////////////////////////////////////////////////////////////////

class FwSQLiteDBLock
{
public:
    FwSQLiteDBLock(FwSQLiteDatabase* db);
    ~FwSQLiteDBLock();

    inline FwSQLiteDatabase* db() const;

    bool lock() const;
    bool tryLock() const;

    void unlock();

private:
    mutable bool m_lock;
    FwSQLiteDatabase* m_db;
};

FwSQLiteDatabase* FwSQLiteDBLock::db() const
{
    return m_db;
}

#endif // FIREWORKS_SQLITEDATABASE_H
