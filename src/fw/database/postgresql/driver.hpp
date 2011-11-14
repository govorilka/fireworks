#ifndef FW_DATABASE_POSTGRESQL_DRIVER_HPP
#define FW_DATABASE_POSTGRESQL_DRIVER_HPP

#include "fw/database/postgresql/defs.hpp"
#include "fw/database/driver.hpp"
#include "fw/database/postgresql/querydata.hpp"

class FIREWORKSSHARED_EXPORT Fw::Database::PostgreSQL::Driver : public Fw::Database::Driver
{
    typedef Fw::Database::Driver BaseClass;

    friend class QueryData;

public:
    Driver(const QByteArray& name);
    virtual ~Driver();

    virtual bool loadData(FwMLObject* object);
    virtual void resetData();

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

protected:
    virtual void init() throw(Fw::Exception&);
    virtual void release() throw();

    virtual int lastInsertKey();

    virtual QueryData* createQuery(const QString& query) throw(Fw::Exception&);

    QString lastError() const;

private:
    PGconn* m_connection;

    int m_lastInsertRowId;

    struct ConnectionParams;
    ConnectionParams* m_conParams;
};

#endif // FW_DATABASE_POSTGRESQL_DRIVER_HPP
