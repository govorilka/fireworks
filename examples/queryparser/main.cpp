#include <QtCore/qcoreapplication.h>
#include <QtCore/qdebug.h>

#include "fwdb/fwpg.h"

extern void ConnectPG(const char*const query);
int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv); 

    qDebug() << "#Parse begin" << endl;
    QString query("select * from  ?? where id = ?1 and name=?2");

    FwPg::TokenVector tokens;
    FwPg::parseQuery(query.toUtf8(), tokens);

    qDebug() << query << endl;
    foreach(FwPg::QueryToken token, tokens)
    {
        qDebug() << token.param << token.value;
    }

    qDebug() << endl << "#Parse end" << endl;

    QString connection;
    if(argc > 1)
    {
        connection = argv[1];
    }

    //ConnectPG(connection.toUtf8().constData());

    FwPg::Database pgdb;
    Fw::Database& db = pgdb;

    try
    {
        db.open(connection);

        Fw::Query query1 = db.query("BEGIN");
        query1.step();

        Fw::Query query2 = db.query("DECLARE myportal CURSOR FOR select * from pg_database");
        query2.step();

        Fw::Query query3 = db.query("FETCH ALL in myportal");
        while(query3.step())
        {
            printf("%-15s", query3.columnText(0).toUtf8().constData());
        }

        Fw::Query query4 = db.query("CLOSE myportal");
        query4.step();

        Fw::Query query5 = db.query("END");
        query5.step();

        db.close();
    }
    catch(Fw::Exception& e)
    {
        QString err = e.what();
        qDebug() << "Exception occured:" << err.toUtf8().constData();
        //printf("Exception occured: %s", e.what());
    }

    return 0;
}

static void
exit_nicely(PGconn *conn)
{
    PQfinish(conn);
    exit(1);
}

void ConnectPG(const char*const query)
{
    {
        const char *conninfo;
        PGconn     *conn;
        PGresult   *res;
        int         nFields;
        int         i,
                    j;

        /*
         * If the user supplies a parameter on the command line, use it as the
         * conninfo string; otherwise default to setting dbname=postgres and using
         * environment variables or defaults for all other connection parameters.
         */
        if (query)
            conninfo = query;
        else
            conninfo = "dbname = postgres";

        /* Make a connection to the database */
        conn = PQconnectdb(conninfo);

        /* Check to see that the backend connection was successfully made */
        if (PQstatus(conn) != CONNECTION_OK)
        {
            fprintf(stderr, "Connection to database failed: %s",
                    PQerrorMessage(conn));
            exit_nicely(conn);
        }

        /*
         * Our test case here involves using a cursor, for which we must be inside
         * a transaction block.  We could do the whole thing with a single
         * PQexec() of "select * from pg_database", but that's too trivial to make
         * a good example.
         */

        /* Start a transaction block */
        res = PQexec(conn, "BEGIN");
        if (PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            fprintf(stderr, "BEGIN command failed: %s", PQerrorMessage(conn));
            PQclear(res);
            exit_nicely(conn);
        }

        /*
         * Should PQclear PGresult whenever it is no longer needed to avoid memory
         * leaks
         */
        PQclear(res);

        /*
         * Fetch rows from pg_database, the system catalog of databases
         */
        res = PQexec(conn, "DECLARE myportal CURSOR FOR select * from pg_database");
        if (PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            fprintf(stderr, "DECLARE CURSOR failed: %s", PQerrorMessage(conn));
            PQclear(res);
            exit_nicely(conn);
        }
        PQclear(res);

        res = PQexec(conn, "FETCH ALL in myportal");
        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            fprintf(stderr, "FETCH ALL failed: %s", PQerrorMessage(conn));
            PQclear(res);
            exit_nicely(conn);
        }

        /* first, print out the attribute names */
        nFields = PQnfields(res);
        for (i = 0; i < nFields; i++)
            printf("%-15s", PQfname(res, i));
        printf("\n\n");

        /* next, print out the rows */
        for (i = 0; i < PQntuples(res); i++)
        {
            for (j = 0; j < nFields; j++)
                printf("%-15s", PQgetvalue(res, i, j));
            printf("\n");
        }

        PQclear(res);

        /* close the portal ... we don't bother to check for errors ... */
        res = PQexec(conn, "CLOSE myportal");
        PQclear(res);

        /* end the transaction */
        res = PQexec(conn, "END");
        PQclear(res);

        /* close the connection to the database and cleanup */
        PQfinish(conn);

        return;
    }
}
