#ifndef FIREWORKS_SQLITE_H
#define FIREWORKS_SQLITE_H

#include <QtCore/qobject.h>
#include <QtCore/qsharedpointer.h>
#include <QtCore/qurl.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qreadwritelock.h>

#include <QtGui/qcolor.h> //TEMPORARY

#include "fwdb/sqlite/sqlite3.h"

class FwSQLiteDatabase;

class FwSQLiteException : public std::exception
{
    typedef std::exception BaseClass;

public:
    FwSQLiteException(const QString& err) throw();
    FwSQLiteException(sqlite3 *db) throw();
    virtual ~FwSQLiteException() throw();

    virtual const char* what() const throw();

    QString error;
};

////////////////////////////////////////////////////////////////////////////////

class FwSQLiteQueryData
{
public:
    friend class FwSQLiteDatabase;

    ~FwSQLiteQueryData();

    inline FwSQLiteDatabase* parent() const;

    sqlite3 *db;
    sqlite3_stmt* stmt;

    void finalize();

protected:
    FwSQLiteQueryData(FwSQLiteDatabase* parent, sqlite3 *pDB, sqlite3_stmt* pStmt);

private:
    FwSQLiteDatabase* m_parent;
};

FwSQLiteDatabase* FwSQLiteQueryData::parent() const
{
    return m_parent;
}

////////////////////////////////////////////////////////////////////////////////

class FwSQLiteQuery : protected QSharedPointer<FwSQLiteQueryData>
{
    typedef QSharedPointer<FwSQLiteQueryData> BaseClass;

public:
    friend class FwSQLiteDatabase;

    FwSQLiteQuery();

    bool isNull() const;

    bool operator==(const FwSQLiteQuery &other) const;
    bool operator!=(const FwSQLiteQuery &other) const;

    bool step() throw (FwSQLiteException&);

    QString columnText(int column);
    int columnInt(int column);
    QUrl columnUrl(int column);
    bool columnBool(int column);
    QColor columnColor(int column);

    void finalize();

    inline void bindUrl(int index, const QUrl& url);
    void bindText(int index, const QString& text);
    void bindInt(int index, int value);
    void bindDateTime(int index, const QDateTime& datetime);
    void bindColor(int index, const  QColor& color);

    void reset();

protected:
    FwSQLiteQuery(FwSQLiteQueryData * data);
};

void FwSQLiteQuery::bindUrl(int index, const QUrl& url)
{
    bindText(index, url.toString());
}

////////////////////////////////////////////////////////////////////////////////

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

#endif // FIREWORKS_SQLITE_H
