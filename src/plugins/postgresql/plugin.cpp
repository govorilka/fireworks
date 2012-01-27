#include <QtCore/qplugin.h>

#include "plugins/postgresql/plugin.hpp"
#include "plugins/postgresql/driver.hpp"

Fw::Database::Driver* Fw::Database::PostgreSQL::Plugin::createDriver() const throw(const Fw::Exception&)
{
    return new Driver("Fw::Database::PostgreSQL::Plugin");
}

void Fw::Database::PostgreSQL::Plugin::deleteDriver(Fw::Database::Driver* driver) const
{
    delete driver;
    driver = 0;
}

Fw::Database::PostgreSQL::Plugin::Plugin(QObject* parent) :
    BaseClass(parent)
{

}

Fw::Database::PostgreSQL::Plugin::~Plugin()
{
}

Q_EXPORT_PLUGIN2(postgreImpl, Fw::Database::PostgreSQL::Plugin)
