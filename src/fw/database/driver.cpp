#include "driver.hpp"
#include "query.hpp"

Fw::Database::Driver::Driver(const QByteArray& name) :
    BaseClass(name),
    m_beginTransaction(false)
{
}

Fw::Database::Driver::~Driver()
{
}

void Fw::Database::Driver::beginTransaction() throw(const Fw::Exception&)
{
    if(!m_beginTransaction)
    {
        QueryPtr query = createQuery(DriverPtr(this), "BEGIN");
        query->step();
        m_beginTransaction = true;
    }
}

void Fw::Database::Driver::commit() throw(const Fw::Exception&)
{
    if(m_beginTransaction)
    {
        createQuery(DriverPtr(this), "COMMIT")->step();
        m_beginTransaction = false;
    }
}

void Fw::Database::Driver::rollback() throw(const Fw::Exception&)
{
    if(m_beginTransaction)
    {
        createQuery(DriverPtr(this), "ROLLBACK")->step();
        m_beginTransaction = false;
    }
}

void Fw::Database::Driver::reindex(const QString& indexName) throw(const Fw::Exception&)
{
    createQuery(DriverPtr(this), QString("REINDEX %1").arg(indexName))->step();
}

void Fw::Database::Driver::execFile(const QString& fileName) throw(const Fw::Exception&)
{
    QFile sqlFile(fileName);
    execFile(&sqlFile);
}

void Fw::Database::Driver::execFile(QIODevice* device) throw(const Fw::Exception&)
{
    if(!device->open(QIODevice::ReadOnly))
    {
        throw Fw::Exception(device->errorString().toUtf8());
    }

    beginTransaction();

    QList<QByteArray> commands = device->readAll().simplified().split(';');
    foreach(QByteArray command, commands)
    {
        if(!command.isEmpty())
        {
            createQuery(DriverPtr(this), QString::fromUtf8(command))->step();
        }
    }

    commit();
}

QString Fw::Database::Driver::lastError() const
{
    return "Cannot connect to database";
}
