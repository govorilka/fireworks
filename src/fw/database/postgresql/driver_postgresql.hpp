#ifndef FW_DATABASE_POSTGRESQL_DRIVER_HPP
#define FW_DATABASE_POSTGRESQL_DRIVER_HPP

#ifdef FW_SUPPORT_POSTGRESQL

#include "fw/database/driver.hpp"

#include "fw/database/postgresql/defs.hpp"
#include "fw/database/postgresql/query_postgresql.hpp"

class FIREWORKSSHARED_EXPORT Fw::Database::PostgreSQL::Driver : public Fw::Database::Driver
{
private:
    typedef Fw::Database::Driver BaseClass;

    PGconn* m_connection;

    int m_lastInsertRowId;

    struct ConnectionParams;
    ConnectionParams* m_conParams;

    bool parseQuery(const QByteArray& query, Fw::Database::PostgreSQL::TokenVector& tokens);

public:
    explicit Driver(const QByteArray& name);
    virtual ~Driver();

    virtual bool loadData(FwMLObject* script) throw(const Fw::Exception&);
    virtual void resetData();

    virtual void open() throw(const Exception&);
    virtual void close() throw();

    virtual void execSimpleQuery(const QString& query) throw(const Fw::Exception&);
    virtual QueryPtr createQuery(const DriverPtr& driver, const QString& query) throw(const Fw::Exception&);

    QString lastError() const;
    virtual bool isOpen() const;
    virtual int lastInsertKey() const;
    void setLastInsertKey(int key);

    const PGconn* connection() const;
    PGconn* connection();

    void setHost(const QByteArray& host);
    void setPort(int port);
    void setDbName(const QByteArray& dbname);
    void setUser(const QByteArray& user);
    void setPassword(const QByteArray& password);

    const QByteArray& getHost() const;
    int getPort() const;
    const QByteArray& getDbName() const;
    const QByteArray& getUser() const;
    const QByteArray& getPassword() const;
};

//class FIREWORKSSHARED_EXPORT Fw::Database::PostgreSQL::Driver : public Fw::Database::Driver
//{
//    typedef Fw::Database::Driver BaseClass;

//    friend class QueryData;

//protected:
//    virtual int lastInsertKey();

//    QString lastError() const;

//public:
//    Driver(const QByteArray& name);
//    virtual ~Driver();

//    virtual bool loadData(FwMLObject* object);
//    virtual void resetData();

//    void setHost(const QByteArray& host);
//    void setPort(int port);
//    void setDbName(const QByteArray& dbname);
//    void setUser(const QByteArray& user);
//    void setPassword(const QByteArray& password);

//    const QByteArray& getHost() const;
//    int getPort() const;
//    const QByteArray& getDbName() const;
//    const QByteArray& getUser() const;
//    const QByteArray& getPassword() const;

//    virtual void open() throw(Fw::Exception&);
//    virtual void close() throw();

//    virtual Fw::Database::QueryData* query(const QString& query) const throw(Fw::Exception&);

//protected:
//    virtual int lastInsertKey();

//    QString lastError() const;

//private:
//    PGconn* m_connection;

//    int m_lastInsertRowId;

//    struct ConnectionParams;
//    ConnectionParams* m_conParams;
//};

#endif //FW_SUPPORT_POSTGRESQL

#endif // FW_DATABASE_POSTGRESQL_DRIVER_HPP
