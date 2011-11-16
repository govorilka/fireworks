#ifndef FIREWORKS_DATABASE_POSTGRESQL_QUERYDATA_HPP
#define FIREWORKS_DATABASE_POSTGRESQL_QUERYDATA_HPP

#ifdef FW_SUPPORT_POSTGRESQL

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
            struct QueryToken;
            typedef QVector<QueryToken> TokenVector;
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

class FIREWORKSSHARED_EXPORT Fw::Database::PostgreSQL::Query : public Fw::Database::Query
{
private:
    typedef Fw::Database::Query BaseClass;

    PGresult* m_result;
    TokenVector m_tokens;
    int m_countRow;
    int m_currRow;

    int m_lastInsertRowId;

protected:
    virtual bool doExec() throw (const Fw::Exception&);
    virtual bool doNext() throw (const Fw::Exception&);
    virtual void doReset();

    void closeQuery();

public:
     explicit Query(const DriverPtr& driver, const TokenVector& query);
     explicit Query(const DriverPtr& driver, PGresult* result);
     virtual ~Query();

     bool operator==(const Query& other) const;
     bool operator!=(const Query& other) const;

     virtual void bindInt(int index, int value) throw(const Fw::Exception&);
     virtual void bindText(int index, const QString& text) throw(const Fw::Exception&);
     virtual void bindDateTime(int index, const QDateTime& dateTime) throw (const Fw::Exception&);
     virtual void bindDate(int index, const QDate& date) throw (const Fw::Exception&);
     virtual void bindTime(int index, const QTime& time) throw (const Fw::Exception&);

     virtual bool columnBool(int column) const throw (const Fw::Exception&);
     virtual int columnInt(int column) const throw (const Fw::Exception&);
     virtual QString columnText(int column) const throw (const Fw::Exception&);
     virtual FwColor columnColor(int column) const throw (const Fw::Exception&);
     virtual QUrl columnUrl(int column) const throw (const Fw::Exception&);
     virtual QDateTime columnDateTime(int column) const throw (const Fw::Exception&);
     virtual QDate columnDate(int column) const throw (const Fw::Exception&);
     virtual QTime columnTime(int column) const throw (const Fw::Exception&);
};

#endif //FW_SUPPORT_POSTGRESQL

//==============================================================================

//class FIREWORKSSHARED_EXPORT Fw::Database::PostgreSQL::QueryData : public Fw::Database::QueryData
//{
//    typedef Fw::Database::QueryData BaseClass;
//    friend class Fw::Database::PostgreSQL::Driver;

//public:
//    ~QueryData();

//    bool isNull() const;

//    bool operator==(const QueryData& other) const;
//    bool operator!=(const QueryData& other) const;

//    virtual bool doExec() throw (const Fw::Exception&);
//    virtual bool doNext() throw (const Fw::Exception&);
//    virtual void doReset();
//    virtual void doFinalize();

//    virtual void doBindInt(int index, int value) throw(const Fw::Exception&);
//    virtual void doBindText(int index, const QString& text) throw(const Fw::Exception&);
//    virtual void doBindDateTime(int index, const QDateTime& dateTime);
//    virtual void doBindDate(int index, const QDate& date);
//    virtual void doBindTime(int index, const QTime& time);

//    virtual bool doColumnBool(int column) const;
//    virtual int doColumnInt(int column) const;
//    virtual QString doColumnText(int column) const;
//    virtual FwColor doColumnColor(int column) const;
//    virtual QUrl doColumnUrl(int column) const;
//    virtual QDateTime doColumnDateTime(int column) const;
//    virtual QDate doColumnDate(int column) const;
//    virtual QTime doColumnTime(int column) const;

//    bool columnBool(int column);
//    int columnInt(int column);
//    QString columnText(int column);
//    FwColor columnColor(int column);
//    QUrl columnUrl(int column);

//protected:
//    QueryData(Driver* db, const TokenVector& query);

//    void exec() throw (const Fw::Exception&);

//private:
//    PGresult* m_result;
//    TokenVector m_tokens;
//    int m_countRow;
//    int m_currRow;

//    int m_lastInsertRowId;

//    void bind();
//    void closeQuery();
//};

#endif // FIREWORKS_DATABASE_POSTGRESQL_QUERYDATA_HPP
