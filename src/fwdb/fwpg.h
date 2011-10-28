#ifndef FIREWORKS_PG_H
#define FIREWORKS_PG_H

#include <QtCore/qvector.h>
#include "../fireworks.h"

#ifdef FW_SUPPORT_POSTGRESQL
#ifdef Q_OS_WIN32
#include <libpq-fe.h>
#else //Q_OS_WIN32
#include "pgsql/libpq-fe.h"
#endif //Q_OS_WIN32
#endif //FW_SUPPORT_POSTGRESQL

#include "fwdb.h"

namespace FwPg
{
    class Exception;
    //class ConnectionParams;
    class QueryData;
    class Database;

    struct QueryToken;
    typedef QVector<QueryToken> TokenVector;

    FIREWORKSSHARED_EXPORT bool parseQuery(const QByteArray& query, TokenVector& tokens);
}

struct FIREWORKSSHARED_EXPORT FwPg::QueryToken
{
    inline QueryToken();

    inline void swapParam();

    int param;
    QByteArray value;
};

FwPg::QueryToken::QueryToken() :
    param(0)
{

}

void FwPg::QueryToken::swapParam()
{
    param = value.toInt();
    value.clear();
}

/////////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT FwPg::Exception: public Fw::Exception
{
    typedef Fw::Exception BaseClass;

public:
    Exception(const Database* db) throw();
    Exception(const QString& error) throw();
    virtual ~Exception() throw();
};

//////////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT FwPg::QueryData : public Fw::QueryData
{
    typedef Fw::QueryData BaseClass;
    friend class FwPg::Database;

public:
    ~QueryData();

    bool isNull() const;

    bool operator==(const QueryData& other) const;
    bool operator!=(const QueryData& other) const;

    virtual bool doExec() throw (Fw::Exception&);
    virtual bool doNext() throw (Fw::Exception&);
    virtual void doReset();
    virtual void doFinalize();

    virtual void doBindInt(int index, int value) throw(Fw::Exception&);
    virtual void doBindText(int index, const QString& text) throw(Fw::Exception&);
    virtual void doBindDateTime(int index, const QDateTime& dateTime);

    virtual bool doColumnBool(int column) const;
    virtual int doColumnInt(int column) const;
    virtual QString doColumnText(int column) const;
    virtual FwColor doColumnColor(int column) const;
    virtual QUrl doColumnUrl(int column) const;

    bool columnBool(int column);
    int columnInt(int column);
    QString columnText(int column);
    FwColor columnColor(int column);
    QUrl columnUrl(int column);

protected:
    QueryData(Database* db, const TokenVector& query);

    void exec() throw (Fw::Exception&);

private:
    PGresult* m_result;
    TokenVector m_tokens;
    int m_count_row;
    int m_curr_row;

    int m_lastInsertRowId; //Name of variable without '_' sign, because it's ugly looks! Please!

    void bind();
    void closeQuery();
};

//////////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT FwPg::Database : public Fw::Database
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

    virtual QueryData* createQuery(const QString& query) throw(Fw::Exception&);

private:
    PGconn* m_connection;

    int m_lastInsertRowId;
};

/////////////////////////////////////////////////////////////////////////////////

//void FwPg::Database::open(const ConnectionParams& params) throw (Exception&)
//{
//    open(params.toByteArray());
//}

//bool FwPg::Database::isOpen() const
//{
//    return m_connection;
//}

#endif //FIREWORKS_PG_H
