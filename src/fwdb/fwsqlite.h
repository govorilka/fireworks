#ifndef FIREWORKS_SQLITE_H
#define FIREWORKS_SQLITE_H

#include <QtCore/qhash.h>
#include <QtCore/qvariant.h>

#include "fwdb.h"

#include "fwdb/sqlite/sqlite3.h"

namespace FwSqlite
{
class Exception;
class QueryData;
class Database;
}

///////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT FwSqlite::Exception : public Fw::Exception
{
    typedef Fw::Exception BaseClass;

public:
    Exception(const Database* db) throw();
    Exception(const QString& error) throw();
    virtual ~Exception() throw();
};

////////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT FwSqlite::QueryData : public Fw::QueryData
{
    typedef Fw::QueryData BaseClass;
    friend class FwSqlite::Database;

public:
    ~QueryData();

    bool isNull() const;

    bool operator==(const QueryData& other) const;
    bool operator!=(const QueryData& other) const;

protected:
    QueryData(Database* db, const QByteArray& query) throw(Fw::Exception&);

    virtual bool doExec() throw (Fw::Exception&);
    virtual bool doNext() throw (Fw::Exception&);
    virtual void doReset();
    virtual void doFinalize();

    virtual void doBindInt(int index, int value) throw(Fw::Exception&);
    virtual void doBindText(int index, const QString& text) throw(Fw::Exception&);
    virtual void doBindDateTime(int index, const QDateTime& dateTime) throw(Fw::Exception&);

    virtual bool doColumnBool(int column) const;
    virtual int doColumnInt(int column) const;
    virtual QString doColumnText(int column) const;
    virtual FwColor doColumnColor(int column) const;
    virtual QUrl doColumnUrl(int column) const;

private:
    sqlite3_stmt* m_stmt;
};

///////////////////////////////////////////////////////////////////////////////

//class FIREWORKSSHARED_EXPORT FwSQLiteQueryData
//{
//public:
//    friend class FwSQLiteDatabase;

//    ~FwSQLiteQueryData();

//    inline FwSQLiteDatabase* parent() const;

//    sqlite3 *db;
//    sqlite3_stmt* stmt;

//    void finalize();

//protected:
//    FwSQLiteQueryData(FwSQLiteDatabase* parent, sqlite3 *pDB, sqlite3_stmt* pStmt);

//private:
//    FwSQLiteDatabase* m_parent;
//};

//FwSQLiteDatabase* FwSQLiteQueryData::parent() const
//{
//    return m_parent;
//}

////////////////////////////////////////////////////////////////////////////////

//class FIREWORKSSHARED_EXPORT FwSQLiteQuery : protected QSharedPointer<FwSQLiteQueryData>
//{
//    typedef QSharedPointer<FwSQLiteQueryData> BaseClass;

//public:
//    friend class FwSQLiteDatabase;

//    FwSQLiteQuery();

//    bool isNull() const;

//    bool operator==(const FwSQLiteQuery &other) const;
//    bool operator!=(const FwSQLiteQuery &other) const;

//    bool step() throw (FwSQLiteException&);

//    QString columnText(int column);
//    int columnInt(int column);
//    QUrl columnUrl(int column);
//    bool columnBool(int column);
//    FwColor columnColor(int column);

//    void finalize();

//    inline void bindUrl(int index, const QUrl& url);
//    void bindText(int index, const QString& text);
//    void bindInt(int index, int value);
//    void bindDateTime(int index, const QDateTime& datetime);
//    void bindColor(int index, const FwColor& color);

//    void reset();

//protected:
//    FwSQLiteQuery(FwSQLiteQueryData * data);
//};

//void FwSQLiteQuery::bindUrl(int index, const QUrl& url)
//{
//    bindText(index, url.toString());
//}

////////////////////////////////////////////////////////////////////////////////

//class FIREWORKSSHARED_EXPORT FwSQLiteDatabase : public QObject
//{
//    Q_OBJECT
//    typedef QObject BaseClass;
//public:
//    friend class FwSQLiteQueryData;

//    FwSQLiteDatabase(QObject* parent = 0);
//    virtual ~FwSQLiteDatabase();

//    void open(const QString& fileName, const QString& initScript = QString()) throw(FwSQLiteException&);
//    void open(const QString& fileName, const QString& initScript, int flags) throw(FwSQLiteException&);
//    inline bool isOpen() const;

//    void close();

//    FwSQLiteQuery query(const QString& query) throw(FwSQLiteException&);

//    void exec(const QString& query) throw(FwSQLiteException&);

//    void execFile(const QString& fileName) throw(FwSQLiteException&);
//    void execFile(QIODevice* device) throw(FwSQLiteException&);

//    void beginTransaction() throw(FwSQLiteException&);
//    void commit() throw(FwSQLiteException&);
//    void rollback() throw(FwSQLiteException&);

//    inline bool isTransactionBegin() const;

//    int lastInsertKey() const;

//    void reindex(const QString& indexName) throw(FwSQLiteException&);

//private:
//    sqlite3 *m_db;
//    bool m_beginTransaction;
//    QList<FwSQLiteQueryData*> queries;
//};

//bool FwSQLiteDatabase::isTransactionBegin() const
//{
//    return m_beginTransaction;
//}

//bool FwSQLiteDatabase::isOpen() const
//{
//    return m_db;
//}

////////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT FwSqlite::Database : public Fw::Database
{
    Q_OBJECT
    typedef Fw::Database BaseClass;

    friend class Exception;
    friend class QueryData;

public:
    Database(QObject* parent = 0);
    virtual ~Database();

    virtual bool loadData(FwMLObject* object);


protected:
    virtual bool init(FwMLObject* object, bool* createdDB = 0) throw(Fw::Exception&);
    virtual void release() throw();

    virtual int lastInsertKey();

    virtual Fw::QueryData* createQuery(const QString& query) throw(Fw::Exception&);

private:
    sqlite3* m_connection;
    QHash<QString, QVariant> m_parameters;

};
#endif // FIREWORKS_SQLITE_H
