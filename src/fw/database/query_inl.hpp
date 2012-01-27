#ifndef FIREWORKS_DATABASE_QUERY_INL_HPP
#define FIREWORKS_DATABASE_QUERY_INL_HPP

#include "fw/database/query.hpp"

const Fw::Database::Driver* Fw::Database::Query::driver() const
{
    return m_driverLoader->driver();
}

Fw::Database::Driver* Fw::Database::Query::driver()
{
    return m_driverLoader->driver();
}

QString Fw::Database::Query::toString() const throw(const Fw::Exception&)
{
    return QString::fromUtf8(toUtf8());
}

#endif // FIREWORKS_DATABASE_QUERY_INL_HPP
