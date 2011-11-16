#ifndef FIREWORKS_DATABASE_QUERY_INL_HPP
#define FIREWORKS_DATABASE_QUERY_INL_HPP

#include "fw/database/query.hpp"

const Fw::Database::Driver* Fw::Database::Query::driver() const
{
    return m_driver.data();
}

Fw::Database::Driver* Fw::Database::Query::driver()
{
    return m_driver.data();
}

//Fw::Database::Controller* Fw::Database::QueryData::controller() const
//{
//    return m_controller;
//}

//bool Fw::Database::QueryData::isExec() const
//{
//    return m_exec;
//}

/////////////////////////////////////////////////////////////////////////////////

//bool Fw::Database::Query::step() throw (Fw::Exception&)
//{
//    return getQueryData()->step();
//}

//void Fw::Database::Query::reset()
//{
//    getQueryData()->reset();
//}

//void Fw::Database::Query::bindInt(int index, int value) throw(Exception&)
//{
//    getBindQueryData()->doBindInt(index, value);
//}

//void Fw::Database::Query::bindText(int index, const QString& text) throw(Exception&)
//{
//    getBindQueryData()->doBindText(index, text);
//}

//void Fw::Database::Query::bindColor(int index, const FwColor& color) throw(Exception&)
//{
//    getBindQueryData()->doBindInt(index, static_cast<qint32>(color.argb()));
//}

//void Fw::Database::Query::bindUrl(int index, const QUrl& url)
//{
//    getBindQueryData()->doBindText(index, url.toString());
//}

//void Fw::Database::Query::bindDateTime(int index, const QDateTime& datetime) throw(Exception&)
//{
//    getBindQueryData()->doBindDateTime(index, datetime);
//}

//void Fw::Database::Query::bindDate(int index, const QDate& date) throw(Exception&)
//{
//    getBindQueryData()->doBindDate(index, date);
//}

//void Fw::Database::Query::bindTime(int index, const QTime& time) throw(Exception&)
//{
//    getBindQueryData()->doBindTime(index, time);
//}

//bool Fw::Database::Query::columnBool(int column) const
//{
//    return getQueryData()->doColumnBool(column);
//}

//int Fw::Database::Query::columnInt(int column) const
//{
//    return getQueryData()->doColumnInt(column);
//}

//QString Fw::Database::Query::columnText(int column) const
//{
//    return getQueryData()->doColumnText(column);
//}

//FwColor Fw::Database::Query::columnColor(int column) const
//{
//    return getQueryData()->doColumnColor(column);
//}

//QUrl Fw::Database::Query::columnUrl(int column) const
//{
//    return getQueryData()->doColumnUrl(column);
//}

//QDateTime Fw::Database::Query::columnDateTime(int column) const
//{
//    return getQueryData()->doColumnDateTime(column);
//}

//QDate Fw::Database::Query::columnDate(int column) const
//{
//    return getQueryData()->doColumnDate(column);
//}

//QTime Fw::Database::Query::columnTime(int column) const
//{
//    return getQueryData()->doColumnTime(column);
//}

#endif // FIREWORKS_DATABASE_QUERY_INL_HPP
