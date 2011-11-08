#ifndef FIREWORKS_SQLITE_H
#define FIREWORKS_SQLITE_H

#include <QtCore/qhash.h>
#include <QtCore/qvariant.h>

#include "fwdb.hpp"

#include "fwdb/sqlite/sqlite3.h"

namespace FwSqlite
{
    class QueryData;
    class Database;
}

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
    virtual void doBindDate(int index, const QDate& date) throw(Fw::Exception&);
    virtual void doBindTime(int index, const QTime& time) throw(Fw::Exception&);

    virtual bool doColumnBool(int column) const;
    virtual int doColumnInt(int column) const;
    virtual QString doColumnText(int column) const;
    virtual FwColor doColumnColor(int column) const;
    virtual QUrl doColumnUrl(int column) const;
    virtual QDateTime doColumnDateTime(int column) const;
    virtual QDate doColumnDate(int column) const;
    virtual QTime doColumnTime(int column) const;

private:
    sqlite3_stmt* m_stmt;
};

////////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT FwSqlite::Database : public Fw::Database
{
    Q_OBJECT
    typedef Fw::Database BaseClass;

    friend class QueryData;

public:
    Database(const QByteArray& name, QObject* parent = 0);
    virtual ~Database();

    virtual bool loadData(FwMLObject* object);

    inline void setPath(const QString& path);
    inline QString path() const;

    inline void setInitPath(const QString& path);
    inline QString initPath() const;

protected:
    virtual void init() throw(Fw::Exception&);
    virtual void release() throw();

    virtual int lastInsertKey();

    virtual Fw::QueryData* createQuery(const QString& query) throw(Fw::Exception&);

    QString lastError() const;

private:
    sqlite3* m_connection;
    QString m_path;
    QString m_initPath;

};

void FwSqlite::Database::setPath(const QString& path)
{
    m_path = path;
}

QString FwSqlite::Database::path() const
{
    return m_path;
}

void FwSqlite::Database::setInitPath(const QString& path)
{
    m_initPath = path;
}

QString FwSqlite::Database::initPath() const
{
    return m_initPath;
}

#endif // FIREWORKS_SQLITE_H
