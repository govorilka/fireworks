#include <QtCore/qcoreapplication.h>

#ifndef FW_SQLITE
#include <stdio.h>
#include <sqlite3.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    int i;
    for(i=0; i<argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int main(int argc, char **argv)
{
    if(argc!=3)
    {
      fprintf(stderr, "Usage: %s DATABASE SQL-STATEMENT\n", argv[0]);
      return 1;
    }

    sqlite3 *db = 0;
    int rc = sqlite3_open(argv[1], &db);
    if(rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    char *zErrMsg = 0;
    rc = sqlite3_exec(db, argv[2], callback, 0, &zErrMsg);
    if(rc!=SQLITE_OK)
    {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }

    sqlite3_close(db);
    return 0;
}
#else
#include "fwdb/dbfactory.h"
#include "fwdb/fwsqlite.h"

int main(int argc, char **argv)
{
    QCoreApplication a(argc, argv);
    Fw::Database* db = Fw::dbFactory(a.applicationDirPath() +"/configure.fwml");
    if(!db)
    {
        return 1;
    }
    Fw::Query insertQuery = db->query("insert into users (userid, login, passwd, birthdate, lastLogin) values (1, 'user_1', '12345', 123456, ?1);");
    insertQuery.bindDateTime(1, QDateTime(QDate(2008, 10, 5), QTime(12, 24, 30)));
    insertQuery.step();
//insert into users ('userid', 'login', 'passwd', ' birthdate', 'lastLogin') values (2, 'user_2', 'abvg', 2356, 12567567);

    return 0;
}

#endif
