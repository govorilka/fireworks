#include <QtCore/qcoreapplication.h>
#include <QtCore/qtextstream.h>

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

    Fw::Query insertQuery = db->query("insert or IGNORE into users (userid, login, passwd, birthdate, lastLogin, time) values (?1, ?2, ?3, ?4, ?5, ?6);");
    insertQuery.bindInt(1,1);
    insertQuery.bindText(2,"user_1");
    insertQuery.bindText(3,"12345");
    insertQuery.bindDate(4, QDate(1990, 5, 10));
    insertQuery.bindDateTime(5, QDateTime(QDate(2008, 10, 5), QTime(12, 24, 30)));
    insertQuery.bindTime(6, QTime(12, 23, 40));
    insertQuery.step();

    insertQuery.bindInt(1,2);
    insertQuery.bindText(2,"user_2");
    insertQuery.bindText(3,"78906557");
    insertQuery.bindDate(4, QDate(1975, 6, 6));
    insertQuery.bindDateTime(5, QDateTime(QDate(2010, 11, 2), QTime(18, 05, 22)));
    insertQuery.bindTime(6, QTime(18, 00, 55));
    insertQuery.step();

    Fw::Query selectQuery = db->query("SELECT * FROM users");
    QTextStream qout(stdout, QIODevice::WriteOnly);

    while(selectQuery.step())
    {
        int id = selectQuery.columnInt(0);
        QString user_login = selectQuery.columnText(1);
        QString passwd = selectQuery.columnText(2);
        QDate birthday = selectQuery.columnDate(3);
        QDateTime last_login = selectQuery.columnDateTime(4);
        QTime time = selectQuery.columnTime(5);
        qout << "user_id: " << id << " login: " << user_login << " passwd: " << passwd <<
                " birthday: " << birthday.toString() << " last login: "<< last_login.toString() <<
              " time: "<< time.toString() << endl;
        qout.flush();
    }


    return 0;
}

#endif
