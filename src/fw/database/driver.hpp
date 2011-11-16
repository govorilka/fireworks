#ifndef FIREWORKS_DATABASE_DRIVER_HPP
#define FIREWORKS_DATABASE_DRIVER_HPP

#include <QtCore/qreadwritelock.h>

#include "fwcore/fwcppobject.h"
#include "fw/database/defs.hpp"

class Fw::Database::Driver : public FwCPPObject
{
    typedef FwCPPObject BaseClass;

    bool m_beginTransaction;

    QReadWriteLock m_dbLock;

public:
    explicit Driver(const QByteArray& name);
    virtual ~Driver();

    inline static void deleter(Driver* driver);
    inline static void emptyDeleter(Driver* driver);

    virtual void open() throw(const Fw::Exception&) = 0;
    virtual void close() throw() = 0;

    virtual QueryPtr createQuery(const DriverPtr& driver, const QString& query) throw(const Fw::Exception&) = 0;

    virtual QString lastError() const = 0;
    virtual bool isOpen() const = 0;
    virtual int lastInsertKey()const  = 0;

    void beginTransaction() throw(const Fw::Exception&);
    void commit() throw(const Fw::Exception&);
    void rollback() throw(const Fw::Exception&);

    void reindex(const QString& indexName) throw(const Fw::Exception&);

    void execFile(const QString& fileName) throw(const Fw::Exception&);
    void execFile(QIODevice* device) throw(const Fw::Exception&);
};

void Fw::Database::Driver::deleter(Driver* driver)
{
    driver->close();
    delete driver;
    driver = 0;
}
void Fw::Database::Driver::emptyDeleter(Driver* driver)
{
    driver = 0;
}

///////////////////////////////////////////////////////////////////////////////

//=============================================================================

//class Fw::Database::Driver : public FwCPPObject
//{
//    typedef FwCPPObject BaseClass;

//public:
//    Driver(const QByteArray& name);
//    virtual ~Driver();

//    virtual void open() throw(Exception&) = 0;
//    virtual void close() throw() = 0;

//    virtual QueryData* query(const QString& q) const throw(Fw::Exception&) = 0;
//};

#endif //FIREWORKS_DATABASE_DRIVER_HPP
