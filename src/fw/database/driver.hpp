#ifndef FIREWORKS_DATABASE_DRIVER_HPP
#define FIREWORKS_DATABASE_DRIVER_HPP

#include <QtCore/qobject.h>
#include <QtCore/qreadwritelock.h>

#include "fw/database/defs.hpp"
#include "fw/helpers/lock.hpp"
#include "fwcore/fwcppobject.h"

namespace Fw
{
    namespace Database
    {
        class DriverFactory;
    }
}

class FIREWORKSSHARED_EXPORT Fw::Database::Driver : public FwCPPObject, public Fw::Helpers::Lockable
{
    typedef FwCPPObject BaseClass;
    friend class Transaction;

    bool m_beginTransaction;

public:
    explicit Driver(const QByteArray& name);
    virtual ~Driver();

    inline static void Deleter(Driver* driver);
    inline static void EmptyDeleter(Driver* driver);

    virtual void open() throw(const Fw::Exception&) = 0;
    virtual void close() throw() = 0;

    virtual QueryPtr createQuery(const DriverLoaderPtr& driver, const QString& query) throw(const Fw::Exception&) = 0;

    virtual QString lastError() const = 0;
    virtual bool isOpen() const = 0;
    virtual int lastInsertKey()const  = 0;

    virtual void execSimpleQuery(const QString& query) throw(const Fw::Exception&) = 0;
    virtual void reindex(const QString& indexName) throw(const Fw::Exception&);

    void execFile(const QString& fileName) throw(const Fw::Exception&);
    void execFile(QIODevice* device) throw(const Fw::Exception&);

protected:
    virtual void transactionBegin() throw(const Fw::Exception&);
    virtual void transactionCommit() throw(const Fw::Exception&);
    virtual void transactionRollback() throw(const Fw::Exception&);
};

void Fw::Database::Driver::Deleter(Driver* driver)
{
    driver->close();
    delete driver;
    driver = 0;
}

void Fw::Database::Driver::EmptyDeleter(Driver* driver)
{
    driver = 0;
}


#endif //FIREWORKS_DATABASE_DRIVER_HPP
