#include "fwcore/fwml.h"

#include "controller.hpp"
#include "driver.hpp"

#include "fw/database/postgresql/driver.hpp"
#include "fw/database/sqlite/driver.hpp"

Fw::Database::Controller::Controller(const QByteArray& name) :
    BaseClass(name)
{
}

Fw::Database::Controller::~Controller()
{
}

Fw::Database::DriverPtr Fw::Database::Controller::factory(FwMLObject* config) throw(const Fw::Exception&)
{
    QByteArray driver = config->baseName().toLower();
    if(driver.isEmpty())
    {
        throw Fw::Exception("Driver name is not defined");
    }

    if(driver == "sqlite")
    {
        return DriverPtr(new Fw::Database::SQLite::Driver("database"), Fw::Database::SQLite::Driver::deleter);
    }
    #ifdef FW_SUPPORT_POSTGRESQL
    else if(driver == "postgresql")
    {
        return DriverPtr(new Fw::Database::PostgreSQL::Driver("database"), Fw::Database::PostgreSQL::Driver::deleter);
    }
    #endif //FW_SUPPORT_POSTGRESQL

    throw  Fw::Exception("Unknow driver name");
    return Fw::Database::DriverPtr();
}

bool Fw::Database::Controller::loadData(FwMLObject* script) throw(const Fw::Exception&)
{
    m_driver.clear();

    FwMLObject* driverNode = script->attribute("driver")->cast<FwMLObject>();
    if(!driverNode)
    {
        throw Fw::Exception("No driver node in fwml object");
    }

    m_driver = factory(driverNode);
    m_driver->loadData(driverNode);

    return true;
}

QString Fw::Database::Controller::loadQueryString(const QString& filename, const QStringList& arguments) throw (const Fw::Exception&)
{
    QFile sqlFile(filename);
    if(!sqlFile.open(QIODevice::ReadOnly))
    {
        throw Fw::Exception(sqlFile);
    }

    QString query = QString::fromUtf8(sqlFile.readAll()).simplified();
    foreach(const QString& arg, arguments)
    {
        query = query.arg(arg);
    }

    return query;
}

Fw::Database::QueryPtr Fw::Database::Controller::createQuery(const QString& query, const QStringList& arguments) throw(const Fw::Exception&)
{
    QString resultQuery = query;
    foreach(const QString& arg, arguments)
    {
        resultQuery = resultQuery.arg(arg);
    }

    return createQuery(resultQuery);
}

//=============================================================================

//Fw::Database::Controller::Controller(const QByteArray& name, QObject* parent) :
//    QObject(parent),
//    BaseClass(name),
//    m_open(false),
//    m_beginTransaction(false),
//    m_driver(0)
//{
//}

//Fw::Database::Controller::~Controller()
//{
//}

//Fw::Database::Query Fw::Database::Controller::query(const QString& query) throw(Exception&)
//{
//    if(!m_driver)
//    {
//        throw Fw::Exception("Database driver is not set");
//    }
//    return Query(m_driver->query(query));
//}

//void Fw::Database::Controller::beginTransaction() throw(Exception&)
//{
//    if(!m_beginTransaction)
//    {
//        Query lquery = query("BEGIN");
//        lquery.step();
//        m_beginTransaction = true;
//    }
//}

//void Fw::Database::Controller::commit() throw(Exception&)
//{
//    if(m_beginTransaction)
//    {
//        Query lquery = query("COMMIT");
//        lquery.step();
//        m_beginTransaction = false;
//    }
//}

//void Fw::Database::Controller::rollback() throw(Exception&)
//{
//    if(m_beginTransaction)
//    {
//        Query lquery = query("ROLLBACK");
//        lquery.step();
//        m_beginTransaction = false;
//    }
//}

//void Fw::Database::Controller::open() throw(Exception&)
//{
//    close();
//    if(!m_driver)
//    {
//        throw Fw::Exception("Database driver is not set");
//    }
//    m_driver->open();
//    m_open = true;
//}

//void Fw::Database::Controller::close() throw()
//{
//    if(m_open)
//    {
//        foreach(QueryData* query, m_queries)
//        {
//            query->release();
//        }
//        m_queries.clear();
//        m_driver->close();
//        m_open = false;
//    }
//}

//void Fw::Database::Controller::reindex(const QString& indexName) throw(Fw::Exception&)
//{
//    Query reindexQuery = query(QString("REINDEX %1").arg(indexName));
//    reindexQuery.step();
//}

//void Fw::Database::Controller::execFile(const QString& fileName) throw(Fw::Exception&)
//{
//    QFile sqlFile(fileName);
//    execFile(&sqlFile);
//}

//void Fw::Database::Controller::execFile(QIODevice* device) throw(Fw::Exception&)
//{
//    if(!device->open(QIODevice::ReadOnly))
//    {
//        throw Fw::Exception(device->errorString().toUtf8());
//    }

//    beginTransaction();

//    QList<QByteArray> commands = device->readAll().simplified().split(';');
//    foreach(QByteArray command, commands)
//    {
//        if(!command.isEmpty())
//        {
//            Query q = query(QString::fromUtf8(command));
//            while(q.step());
//        }
//    }

//    commit();
//}

//bool Fw::Database::Controller::loadData(FwMLObject *object) throw(Fw::Exception&)
//{
//    if(!m_driver)
//    {
//        FwMLObject* driverNode = object->attribute("driver")->cast<FwMLObject>();
//        if(driverNode)
//        {
//            m_driver = factory(driverNode);
//        }
//    }
//    return true;
//}
