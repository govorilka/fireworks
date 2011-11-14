#ifndef FIREWORKS_DATABASE_POSTGRESQL_QUERYDATA_HPP
#define FIREWORKS_DATABASE_POSTGRESQL_QUERYDATA_HPP

#include <QtCore/qvector.h>

#ifdef Q_OS_WIN32
#include <libpq-fe.h>
#else //Q_OS_WIN32
#include "pgsql/libpq-fe.h"
#endif //Q_OS_WIN32

#include "fw/database/query.hpp"
#include "fw/database/postgresql/defs.hpp"

namespace Fw
{
    namespace Database
    {
        namespace PostgreSQL
        {
            class Driver;
            struct QueryToken;
            typedef QVector<QueryToken> TokenVector;

            FIREWORKSSHARED_EXPORT bool parseQuery(const QByteArray& query, TokenVector& tokens);
        }
    }
}

struct FIREWORKSSHARED_EXPORT Fw::Database::PostgreSQL::QueryToken
{
    inline QueryToken();

    inline void swapParam();

    int param;
    QByteArray value;
};

Fw::Database::PostgreSQL::QueryToken::QueryToken() :
    param(0)
{
}

void Fw::Database::PostgreSQL::QueryToken::swapParam()
{
    param = value.toInt();
    value.clear();
}

//////////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT Fw::Database::PostgreSQL::QueryData : public Fw::Database::QueryData
{
    typedef Fw::Database::QueryData BaseClass;
    friend class Fw::Database::PostgreSQL::Driver;

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
    virtual void doBindDate(int index, const QDate& date);
    virtual void doBindTime(int index, const QTime& time);

    virtual bool doColumnBool(int column) const;
    virtual int doColumnInt(int column) const;
    virtual QString doColumnText(int column) const;
    virtual FwColor doColumnColor(int column) const;
    virtual QUrl doColumnUrl(int column) const;
    virtual QDateTime doColumnDateTime(int column) const;
    virtual QDate doColumnDate(int column) const;
    virtual QTime doColumnTime(int column) const;

    bool columnBool(int column);
    int columnInt(int column);
    QString columnText(int column);
    FwColor columnColor(int column);
    QUrl columnUrl(int column);

protected:
    QueryData(Driver* db, const TokenVector& query);

    void exec() throw (Fw::Exception&);

private:
    PGresult* m_result;
    TokenVector m_tokens;
    int m_countRow;
    int m_currRow;

    int m_lastInsertRowId; //Name of variable without '_' sign, because it's ugly looks! Please!

    void bind();
    void closeQuery();
};

#endif // FIREWORKS_DATABASE_POSTGRESQL_QUERYDATA_HPP
