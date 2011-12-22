#ifndef FIREWORKS_DATABASE_POSTGRESQL_QUERYDATA_HPP
#define FIREWORKS_DATABASE_POSTGRESQL_QUERYDATA_HPP

#ifdef FW_SUPPORT_POSTGRESQL

#include <QtCore/qvector.h>

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

     virtual void bindNull(int index) throw(const Fw::Exception&);
     virtual void bindBool(int index, bool value) throw(const Fw::Exception&);
     virtual void bindInt(int index, int value) throw(const Fw::Exception&);
     virtual void bindUInt(int index, uint value) throw(const Fw::Exception&);
     virtual void bindDouble(int index, double value) throw(const Fw::Exception&);
     virtual void bindText(int index, const QString& text) throw(const Fw::Exception&);
     virtual void bindDateTime(int index, const QDateTime& dateTime) throw (const Fw::Exception&);
     virtual void bindDate(int index, const QDate& date) throw (const Fw::Exception&);
     virtual void bindTime(int index, const QTime& time) throw (const Fw::Exception&);
     void bindByteArray(int index, const QByteArray& text);


     virtual bool columnBool(int column) const throw (const Fw::Exception&);
     virtual int columnInt(int column) const throw (const Fw::Exception&);
     virtual uint columnUInt(int column) const throw(const Fw::Exception&);
     virtual double columnDouble(int column) const throw(const Fw::Exception&);
     virtual QString columnText(int column) const throw (const Fw::Exception&);
     virtual FwColor columnColor(int column) const throw (const Fw::Exception&);
     virtual QUrl columnUrl(int column) const throw (const Fw::Exception&);
     virtual QDateTime columnDateTime(int column) const throw (const Fw::Exception&);
     virtual QDate columnDate(int column) const throw (const Fw::Exception&);
     virtual QTime columnTime(int column) const throw (const Fw::Exception&);

     virtual QByteArray toUtf8() const throw(const Fw::Exception&);
};

#endif //FW_SUPPORT_POSTGRESQL

#endif // FIREWORKS_DATABASE_POSTGRESQL_QUERYDATA_HPP