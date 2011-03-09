#ifndef FIREWORKS_SQLITEQUERY_H
#define FIREWORKS_SQLITEQUERY_H

#include <QtCore/qsharedpointer.h>
#include <QtCore/qurl.h>
#include <QtCore/qdatetime.h>

#include <QtGui/qcolor.h>

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

#endif // FIREWORKS_SQLITEQUERY_H
