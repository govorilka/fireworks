#ifndef FIREWORKS_DATABASE_SQLITE_QUERYDATA_HPP
#define FIREWORKS_DATABASE_SQLITE_QUERYDATA_HPP

#include "fw/database/query.hpp"

#include "fw/database/sqlite/defs.hpp"

#include "thirdparty/sqlite/sqlite3.h"

class FIREWORKSSHARED_EXPORT Fw::Database::SQLite::Query : public Fw::Database::Query
{
private:
    typedef Fw::Database::Query BaseClass;

        sqlite3_stmt* m_stmt;

protected:
    virtual bool doExec() throw (const Fw::Exception&);
    virtual bool doNext() throw (const Fw::Exception&);
    virtual void doReset();

    void closeQuery();

public:
    explicit Query(const DriverPtr& driver, sqlite3_stmt* stmt);
    virtual ~Query();

    virtual void bindNull(int index) throw(const Fw::Exception&);
    virtual void bindInt(int index, int value) throw(const Fw::Exception&);
    virtual void bindText(int index, const QString& text) throw(const Fw::Exception&);
    virtual void bindDateTime(int index, const QDateTime& dateTime) throw(const Fw::Exception&);
    virtual void bindDate(int index, const QDate& date) throw(const Fw::Exception&);
    virtual void bindTime(int index, const QTime& time) throw(const Fw::Exception&);

    virtual bool columnBool(int column) const throw(const Fw::Exception&);
    virtual int columnInt(int column) const throw(const Fw::Exception&);
    virtual QString columnText(int column) const throw(const Fw::Exception&);
    virtual FwColor columnColor(int column) const throw(const Fw::Exception&);
    virtual QUrl columnUrl(int column) const throw(const Fw::Exception&);
    virtual QDateTime columnDateTime(int column) const throw(const Fw::Exception&);
    virtual QDate columnDate(int column) const throw(const Fw::Exception&);
    virtual QTime columnTime(int column) const throw(const Fw::Exception&);

    virtual QByteArray toUtf8() const throw(const Fw::Exception&);
};

#endif //FIREWORKS_DATABASE_SQLITE_QUERYDATA_HPP
