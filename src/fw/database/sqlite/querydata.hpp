#ifndef FIREWORKS_DATABASE_SQLITE_QUERYDATA_HPP
#define FIREWORKS_DATABASE_SQLITE_QUERYDATA_HPP

#include "fw/database/query.hpp"

#include "fw/database/sqlite/defs.hpp"

#include "thirdparty/sqlite/sqlite3.h"

class FIREWORKSSHARED_EXPORT Fw::Database::SQLite::QueryData : public Fw::Database::QueryData
{
    typedef Fw::Database::QueryData BaseClass;

public:
    QueryData(Driver* driver, sqlite3_stmt* stmt);
    ~QueryData();

    virtual bool isNull() const;

protected:
    virtual bool doExec() throw (Fw::Exception&);
    virtual bool doNext() throw (Fw::Exception&);
    virtual void doReset();
    virtual void doFinalize();

    virtual void doBindInt(int index, int value) throw(Fw::Exception&);
    virtual void doBindText(int index, const QString& text) throw(Fw::Exception&);
    virtual void doBindDateTime(int index, const QDateTime& dateTime) throw(Fw::Exception&);
    virtual void doBindDate(int index, const QDate& date) throw(Fw::Exception&);
    virtual void doBindTime(int index, const QTime& time) throw(Fw::Exception&);

    virtual bool doColumnBool(int column) const;
    virtual int doColumnInt(int column) const;
    virtual QString doColumnText(int column) const;
    virtual FwColor doColumnColor(int column) const;
    virtual QUrl doColumnUrl(int column) const;
    virtual QDateTime doColumnDateTime(int column) const;
    virtual QDate doColumnDate(int column) const;
    virtual QTime doColumnTime(int column) const;

private:
    sqlite3_stmt* m_stmt;
};

#endif //FIREWORKS_DATABASE_SQLITE_QUERYDATA_HPP
