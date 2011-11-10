#ifndef DRIVER_HPP
#define DRIVER_HPP

#include "fw/database/postgresql/defs.hpp"

#include "fw/database/driver.hpp"

class FIREWORKSSHARED_EXPORT Fw::Database::PostgreSQL::Driver : public Fw::Database::Driver
{
    typedef Fw::Database::Driver BaseClass;

public:
    Driver(const QByteArray& name);

private:
    //PGconn* m_connection;
    //int m_lastInsertRowId;
};

#endif // DRIVER_HPP
