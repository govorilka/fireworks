#ifndef QUERY_INL_HPP
#define QUERY_INL_HPP

Fw::Database::Controller* Fw::Database::QueryData::driver() const
{
    return m_driver;
}

bool Fw::Database::QueryData::isExec() const
{
    return m_exec;
}

///////////////////////////////////////////////////////////////////////////////

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

Fw::Database::Controller* Fw::Database::ControllerLocker::db() const
{
    return m_db;
}

#endif // QUERY_INL_HPP
