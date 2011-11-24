#ifndef FIREWORKS_DATABASE_QUERY_HPP
#define FIREWORKS_DATABASE_QUERY_HPP

#include <QtCore/qdatetime.h>
#include <QtCore/qurl.h>

#include "fwcore/fwcolor.h"

#include "fw/database/defs.hpp"

class  FIREWORKSSHARED_EXPORT Fw::Database::Query
{
    DriverPtr m_driver;
    bool m_exec;

protected:
    virtual bool doExec() throw (const Fw::Exception&) = 0;
    virtual bool doNext() throw (const Fw::Exception&) = 0;
    virtual void doReset() = 0;

    inline const Driver* driver() const;
    inline Driver* driver();

public:
    explicit Query(const DriverPtr& driver);
    virtual ~Query();

    virtual void bindNull(int index) throw(const Fw::Exception&) = 0;
    virtual void bindBool(int index, bool value) throw(const Fw::Exception&);
    virtual void bindInt(int index, int value) throw(const Fw::Exception&) = 0;
    virtual void bindText(int index, const QString& text) throw(const Fw::Exception&) = 0;
    virtual void bindDateTime(int index, const QDateTime& dateTime) throw(const Fw::Exception&) = 0;
    virtual void bindDate(int index, const QDate& date) throw(const Fw::Exception&) = 0;
    virtual void bindTime(int index, const QTime& time) throw(const Fw::Exception&) = 0;

    virtual bool columnBool(int column) const throw(const Fw::Exception&) = 0;
    virtual int columnInt(int column) const throw(const Fw::Exception&) = 0;
    virtual QString columnText(int column) const throw(const Fw::Exception&) = 0;
    virtual FwColor columnColor(int column) const throw(const Fw::Exception&) = 0;
    virtual QUrl columnUrl(int column) const throw(const Fw::Exception&) = 0;
    virtual QDateTime columnDateTime(int column) const throw(const Fw::Exception&) = 0;
    virtual QDate columnDate(int column) const throw(const Fw::Exception&) = 0;
    virtual QTime columnTime(int column) const throw(const Fw::Exception&) = 0;

    virtual QByteArray toUtf8() const throw(const Fw::Exception&) = 0;
    inline virtual QString toString() const throw(const Fw::Exception&);

    bool step() throw (const Fw::Exception&);
    void reset();
};

#include "fw/database/query_inl.hpp"

#endif // FIREWORKS_DATABASE_QUERY_HPP
