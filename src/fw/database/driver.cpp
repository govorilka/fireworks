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

void Fw::Database::Driver::reindex(const QString& indexName) throw(const Fw::Exception&)
{
    execSimpleQuery(QString("REINDEX %1").arg(indexName));
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

    transactionBegin();

    QList<QByteArray> commands = device->readAll().simplified().split(';');
    foreach(QByteArray command, commands)
    {
        if(!command.isEmpty())
        {
            createQuery(DriverPtr(this, emptyDeleter), QString::fromUtf8(command))->step();
        }
    }

    transactionCommit();
}

void Fw::Database::Driver::transactionBegin() throw(const Fw::Exception&)
{
    if(!m_beginTransaction)
    {
        execSimpleQuery("BEGIN");
        m_beginTransaction = true;
    }
}

void Fw::Database::Driver::transactionCommit() throw(const Fw::Exception&)
{
    if(m_beginTransaction)
    {
        execSimpleQuery("COMMIT");
        m_beginTransaction = false;
    }
}

void Fw::Database::Driver::transactionRollback() throw(const Fw::Exception&)
{
    if(m_beginTransaction)
    {
        execSimpleQuery("ROLLBACK");
        m_beginTransaction = false;
    }
}

QString Fw::Database::Driver::lastError() const
{
    return "Cannot connect to database";
}

////////////////////////////////////////////////////////////////////////////////

Fw::Database::Plugin::Plugin(QObject* parent) :
    BaseClass(parent)
{
}
