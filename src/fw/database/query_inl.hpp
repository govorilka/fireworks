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

QString Fw::Database::Query::toString() const throw(const Fw::Exception&)
{
    return QString::fromUtf8(toUtf8());
}

#endif // FIREWORKS_DATABASE_QUERY_INL_HPP
