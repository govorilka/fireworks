#ifndef FW_DATABASE_SQLITE_DRIVER_HPP
#define FW_DATABASE_SQLITE_DRIVER_HPP

#include "fw/database/defs.hpp"
#include "fw/database/driver.hpp"

#include "fw/database/sqlite/defs.hpp"

#include "thirdparty/sqlite/sqlite3.h"

class FIREWORKSSHARED_EXPORT Fw::Database::SQLite::Driver : public Fw::Database::Driver
{
private:
    typedef Fw::Database::Driver BaseClass;

    sqlite3* m_connection;
    QString m_path;
    QString m_initPath;

public:
    explicit Driver(const QByteArray& name);
    virtual ~Driver();

    virtual bool loadData(FwMLObject* script) throw(const Fw::Exception&);

    virtual void open() throw(const Exception&);
    virtual void close() throw();

    virtual QueryPtr createQuery(const DriverPtr& driver, const QString& query) throw(const Fw::Exception&);

    QString lastError() const;
    virtual bool isOpen() const;
    virtual int lastInsertKey();

    inline void setPath(const QString& path);
    inline QString path() const;

    inline void setInitPath(const QString& path);
    inline QString initPath() const;
};

//class FIREWORKSSHARED_EXPORT Fw::Database::SQLite::Driver : public Fw::Database::Driver
//{
//    typedef Fw::Database::Driver BaseClass;

//    friend class QueryData;
//public:
//    Driver(const QByteArray& name);
//    virtual ~Driver();

//    virtual bool loadData(FwMLObject* object);

//    inline void setPath(const QString& path);
//    inline QString path() const;

//    inline void setInitPath(const QString& path);
//    inline QString initPath() const;

//    virtual void open() throw(const Fw::Exception&);
//    virtual void close() throw();

//    virtual Database::QueryData* query(const QString& query) const throw(Fw::Exception&);

//    virtual int lastInsertKey();

//protected:
//    QString lastError() const;

//private:
//    sqlite3* m_connection;
//    QString m_path;
//    QString m_initPath;
//};

void Fw::Database::SQLite::Driver::setPath(const QString& path)
{
    m_path = path;
}

QString Fw::Database::SQLite::Driver::path() const
{
    return m_path;
}

void Fw::Database::SQLite::Driver::setInitPath(const QString& path)
{
    m_initPath = path;
}

QString Fw::Database::SQLite::Driver::initPath() const
{
    return m_initPath;
}

#endif //FW_DATABASE_SQLITE_DRIVER_HPP
