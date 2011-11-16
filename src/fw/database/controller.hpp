#ifndef FIREWORKS_DATABASE_CONTROLLER_HPP
#define FIREWORKS_DATABASE_CONTROLLER_HPP

#include "fwcore/fwcppobject.h"

#include "fw/database/defs.hpp"
#include "fw/database/driver.hpp"

class FIREWORKSSHARED_EXPORT Fw::Database::Controller : public FwCPPObject
{
    typedef FwCPPObject BaseClass;

    DriverPtr m_driver;

    static DriverPtr factory(const FwMLObject& object) throw(const Fw::Exception&);

public:
    explicit Controller(const QByteArray& name);
    virtual ~Controller();

    virtual bool loadData(FwMLObject* script) throw(const Exception&);

    inline QueryPtr createQuery(const QString& query) throw(const Exception&);

    inline const Driver* operator->() const;
    inline Driver* operator->();
};

Fw::Database::QueryPtr Fw::Database::Controller::createQuery(const QString& query) throw(const Exception&)
{
    return m_driver->createQuery(m_driver, query);
}

const Fw::Database::Driver* Fw::Database::Controller::operator->() const
{
    return m_driver.data();
}

Fw::Database::Driver* Fw::Database::Controller::operator->()
{
    return m_driver.data();
}
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
