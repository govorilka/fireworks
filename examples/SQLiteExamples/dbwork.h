#ifndef DBWORK_H
#define DBWORK_H

#ifdef USE_SQLITE_API
#include "fwdb/sqlite/sqlite3.h"
#else
#include "fwdb/fwdb.h"
#include "fwdb/dbfactory.h"
#endif

class DbWork
{
public:
    DbWork();
    bool loadDB(const QString& param);
    bool selectQuery(int index = 0);
    bool selectByText(const QString& param);
    bool insertQuery(const QString& text);


private:
#ifdef USE_SQLITE_API

    bool createQuery(const QString& query);

    bool step();

    void bindInt(int index, int value);
    void bindText(int index, const QString& text);

    bool columnBool(int column) const;
    int columnInt(int column) const;
    QString columnText(int column) const;

    sqlite3* m_connection;
    sqlite3_stmt* m_stmt;
#else //USE_SQLITE_API
    Fw::Database* m_db;
#endif
};

#endif // DBWORK_H
