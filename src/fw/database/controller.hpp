#ifndef FIREWORKS_DATABASE_CONTROLLER_HPP
#define FIREWORKS_DATABASE_CONTROLLER_HPP

#include "fwcore/fwcppobject.h"

#include "fw/database/defs.hpp"
#include "fw/database/driver.hpp"
#include "fw/database/query.hpp"

//class Database
//{
//    class Driver;
//    class Query;
//    class Transaction;
//};

//Fw::Database;
//Fw::Database::Driver;
//Fw::Database::Query;
//Fw::Database::Transaction;

class FIREWORKSSHARED_EXPORT Fw::Database::Controller : public FwCPPObject
{
    typedef FwCPPObject BaseClass;

    DriverPtr m_driver;

    static DriverPtr factory(FwMLObject* config) throw(const Fw::Exception&);

public:
    explicit Controller(const QByteArray& name = "Fw::Database::Controller");
    virtual ~Controller();

    virtual bool loadData(FwMLObject* script) throw(const Fw::Exception&);

    static QString loadQueryString(const QString& filename, const QStringList& arguments = QStringList()) throw(const Fw::Exception&);
    inline QueryPtr loadQuery(const QString& filename, const QStringList& arguments = QStringList()) throw(const Fw::Exception&);
    inline QueryPtr createQuery(const QString& query) throw(const Fw::Exception&);
    QueryPtr createQuery(const QString& query, const QStringList& arguments) throw(const Fw::Exception&);

    inline const Driver& operator*() const throw(const Fw::Exception&);
    inline Driver& operator*() throw(const Fw::Exception&);

    inline const Driver* operator->() const throw(const Fw::Exception&);
    inline Driver* operator->() throw(const Fw::Exception&);

    inline const Driver* driver() const;
    inline Driver* driver();
};

////////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT Fw::Database::Transaction
{
public:
    explicit Transaction(Controller& db) throw(const Fw::Exception&);
    ~Transaction() throw();

    inline bool begin() throw(const Fw::Exception&);
    void commit() throw(const Fw::Exception&);

private:
    Controller& m_db;
    bool m_begin;
};

#define TRANSACTION(db) for(Fw::Database::Transaction t(db); t.begin(); t.commit())

bool Fw::Database::Transaction::begin() throw(const Fw::Exception&)
{
    return m_begin;
}

////////////////////////////////////////////////////////////////////////////////

Fw::Database::QueryPtr Fw::Database::Controller::loadQuery(const QString& filename, const QStringList& arguments) throw(const Fw::Exception&)
{
    return createQuery(loadQueryString(filename, arguments));
}

Fw::Database::QueryPtr Fw::Database::Controller::createQuery(const QString& query) throw(const Fw::Exception&)
{
    return m_driver->createQuery(m_driver, query);
}

const Fw::Database::Driver& Fw::Database::Controller::operator*() const throw(const Fw::Exception&)
{
    const Driver* drv = m_driver.data();
    if(drv == 0)
    {
        throw Fw::Exception(drv->lastError());
    }
    return *drv;
}

Fw::Database::Driver& Fw::Database::Controller::operator*() throw(const Fw::Exception&)
{
    Driver* drv = m_driver.data();
    if(drv == 0)
    {
        throw Fw::Exception(drv->lastError());
    }
    return *drv;
}

const Fw::Database::Driver* Fw::Database::Controller::operator->() const throw(const Fw::Exception&)
{
    Driver* drv = m_driver.data();
    if(drv == 0)
    {
        throw Fw::Exception(drv->lastError());
    }
    return drv;
}

Fw::Database::Driver* Fw::Database::Controller::operator->() throw(const Fw::Exception&)
{
    Driver* drv = m_driver.data();
    if(drv == 0)
    {
        throw Fw::Exception(drv->lastError());
    }
    return drv;
}

const Fw::Database::Driver* Fw::Database::Controller::driver() const
{
    return m_driver.data();
}

Fw::Database::Driver* Fw::Database::Controller::driver()
{
    return m_driver.data();
}

////////////////////////////////////////////////////////////////////////////////

//===================================================================================================

//class FIREWORKSSHARED_EXPORT Fw::Database::Controller : public QObject, public FwCPPObject
//{
//    Q_OBJECT
//    typedef FwCPPObject BaseClass;

//    friend class QueryData;
//    friend class DatabaseLocker;

//public:
//    explicit Controller(const QByteArray& name, QObject* parent = 0);
//    virtual ~Controller();

//    void open() throw(Exception&);
//    void close() throw();
//    inline bool isOpen() const;

//    Query query(const QString& query) throw(Exception&);

//    void beginTransaction() throw(Exception&);
//    void commit() throw(Exception&);
//    void rollback() throw(Exception&);

//    virtual int lastInsertKey() = 0;

//    void reindex(const QString& indexName) throw(Exception&);
//    void execFile(const QString& fileName) throw(Exception&);
//    void execFile(QIODevice* device) throw(Exception&);

//    virtual bool loadData(FwMLObject *object) throw(Fw::Exception&);

//private:
//    bool m_open;
//    bool m_beginTransaction;

//    QReadWriteLock m_dbLock;

//    Driver* m_driver;
//    QList<QueryData*> m_queries;
//};

/////////////////////////////////////////////////////////////////////////////////

//bool Fw::Database::Controller::isOpen() const
//{
//    return m_open;
//}

#endif //FIREWORKS_DATABASE_CONTROLLER_HPP
