#ifndef FIREWORKS_DB_INL_H
#define FIREWORKS_DB_INL_H

#include "fwdb.hpp"

class FIREWORKSSHARED_EXPORT Fw::Query : protected QSharedPointer<Fw::QueryData>
{
    typedef QSharedPointer<Fw::QueryData> BaseClass;

public:
    friend class Database;

    Query();
    ~Query();

    inline bool step() throw (Fw::Exception&);
    inline void reset();

    inline void bindInt(int index, int value) throw(Exception&);
    inline void bindText(int index, const QString& text) throw(Exception&);
    inline void bindColor(int index, const FwColor& color) throw(Exception&);
    inline void bindUrl(int index, const QUrl& url);
    inline void bindDateTime(int index, const QDateTime& datetime) throw(Exception&);
    inline void bindDate(int index, const QDate& date) throw(Exception&);
    inline void bindTime(int index, const QTime& time) throw(Exception&);

    inline bool columnBool(int column) const;
    inline int columnInt(int column) const;
    inline QString columnText(int column) const;
    inline FwColor columnColor(int column) const;
    inline QUrl columnUrl(int column) const;
    inline QDateTime columnDateTime(int column) const;
    inline QDate columnDate(int column) const;
    inline QTime columnTime(int column) const;


protected:
    Query(QueryData* data);

    static void doDeleteQueryData(QueryData* data);

    QueryData* getQueryData() const throw (Fw::Exception&);
    QueryData* getBindQueryData() const throw (Fw::Exception&);
};

bool Fw::Query::step() throw (Fw::Exception&)
{
    return getQueryData()->step();
}

void Fw::Query::reset()
{
    getQueryData()->reset();
}

void Fw::Query::bindInt(int index, int value) throw(Exception&)
{
    getBindQueryData()->doBindInt(index, value);
}

void Fw::Query::bindText(int index, const QString& text) throw(Exception&)
{
    getBindQueryData()->doBindText(index, text);
}

void Fw::Query::bindColor(int index, const FwColor& color) throw(Exception&)
{
    getBindQueryData()->doBindInt(index, static_cast<qint32>(color.argb()));
}

void Fw::Query::bindUrl(int index, const QUrl& url)
{
    getBindQueryData()->doBindText(index, url.toString());
}

void Fw::Query::bindDateTime(int index, const QDateTime& datetime) throw(Exception&)
{
    getBindQueryData()->doBindDateTime(index, datetime);
}

void Fw::Query::bindDate(int index, const QDate& date) throw(Exception&)
{
    getBindQueryData()->doBindDate(index, date);
}

void Fw::Query::bindTime(int index, const QTime& time) throw(Exception&)
{
    getBindQueryData()->doBindTime(index, time);
}

bool Fw::Query::columnBool(int column) const
{
    return getQueryData()->doColumnBool(column);
}

int Fw::Query::columnInt(int column) const
{
    return getQueryData()->doColumnInt(column);
}

QString Fw::Query::columnText(int column) const
{
    return getQueryData()->doColumnText(column);
}

FwColor Fw::Query::columnColor(int column) const
{
    return getQueryData()->doColumnColor(column);
}

QUrl Fw::Query::columnUrl(int column) const
{
    return getQueryData()->doColumnUrl(column);
}

QDateTime Fw::Query::columnDateTime(int column) const
{
    return getQueryData()->doColumnDateTime(column);
}

QDate Fw::Query::columnDate(int column) const
{
    return getQueryData()->doColumnDate(column);
}

QTime Fw::Query::columnTime(int column) const
{
    return getQueryData()->doColumnTime(column);
}

///////////////////////////////////////////////////////////////////////////////

bool Fw::Database::isOpen() const
{
    return m_open;
}

///////////////////////////////////////////////////////////////////////////////

Fw::Database* Fw::DatabaseLocker::db() const
{
    return m_db;
}

#endif // FIREWORKS_DB_INL_H
