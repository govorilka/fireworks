#ifndef FIREWORKS_DATABASE_DRIVER_HPP
#define FIREWORKS_DATABASE_DRIVER_HPP

#include "fwcore/fwcppobject.h"

#include "fw/database/defs.hpp"

class Fw::Database::Driver : public FwCPPObject
{
    typedef FwCPPObject BaseClass;

public:
    Driver(const QByteArray& name);
    virtual ~Driver();

    virtual void open() throw(Exception&) = 0;
    virtual void close() throw() = 0;

    virtual QueryData* query(const QString& q) const throw(Fw::Exception&) = 0;
};

#endif //FIREWORKS_DATABASE_DRIVER_HPP
