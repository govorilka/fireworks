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

    bool step() throw (const Fw::Exception&);
    void reset();
};

//=============================================================================

//class FIREWORKSSHARED_EXPORT Fw::Database::QueryData
//{
//public:
//    friend class Query;
//    friend class Driver;

//    virtual ~QueryData();

//    void release();

//    inline Controller* controller() const;
//    inline bool isExec() const;

//protected:
//    QueryData(Controller* controller);

//    virtual bool doExec() throw (Fw::Exception&) = 0;
//    virtual bool doNext() throw (Fw::Exception&) = 0;
//    virtual void doReset() = 0;
//    virtual void doFinalize() = 0;

//    virtual void doBindInt(int index, int value) throw(Exception&) = 0;
//    virtual void doBindText(int index, const QString& text) throw(Exception&) = 0;
//    virtual void doBindDateTime(int index, const QDateTime& dateTime) = 0;
//    virtual void doBindDate(int index, const QDate& date) = 0;
//    virtual void doBindTime(int index, const QTime& time) = 0;

//    virtual bool doColumnBool(int column) const = 0;
//    virtual int doColumnInt(int column) const = 0;
//    virtual QString doColumnText(int column) const = 0;
//    virtual FwColor doColumnColor(int column) const = 0;
//    virtual QUrl doColumnUrl(int column) const = 0;
//    virtual QDateTime doColumnDateTime(int column) const = 0;
//    virtual QDate doColumnDate(int column) const = 0;
//    virtual QTime doColumnTime(int column) const = 0;

//    bool step() throw (Fw::Exception&);
//    void reset();

//private:
//    Controller* m_controller;
//    bool m_exec;
//};

/////////////////////////////////////////////////////////////////////////////////

//class FIREWORKSSHARED_EXPORT Fw::Database::Query : protected QSharedPointer<QueryData>
//{
//    typedef QSharedPointer<QueryData> BaseClass;

//public:
//    friend class Controller;

//    Query();
//    ~Query();

//    inline bool step() throw (Fw::Exception&);
//    inline void reset();

//    inline void bindInt(int index, int value) throw(Exception&);
//    inline void bindText(int index, const QString& text) throw(Exception&);
//    inline void bindColor(int index, const FwColor& color) throw(Exception&);
//    inline void bindUrl(int index, const QUrl& url);
//    inline void bindDateTime(int index, const QDateTime& datetime) throw(Exception&);
//    inline void bindDate(int index, const QDate& date) throw(Exception&);
//    inline void bindTime(int index, const QTime& time) throw(Exception&);

//    inline bool columnBool(int column) const;
//    inline int columnInt(int column) const;
//    inline QString columnText(int column) const;
//    inline FwColor columnColor(int column) const;
//    inline QUrl columnUrl(int column) const;
//    inline QDateTime columnDateTime(int column) const;
//    inline QDate columnDate(int column) const;
//    inline QTime columnTime(int column) const;

//protected:
//    Query(QueryData* data);

//    static void doDeleteQueryData(QueryData* data);

//    QueryData* getQueryData() const throw (Fw::Exception&);
//    QueryData* getBindQueryData() const throw (Fw::Exception&);
//};

#include "fw/database/query_inl.hpp"

#endif // FIREWORKS_DATABASE_QUERY_HPP
