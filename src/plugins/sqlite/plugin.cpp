#include <QtCore/qplugin.h>

#include "plugins/sqlite/plugin.hpp"
#include "plugins/sqlite/driver.hpp"

Fw::Database::Driver* Fw::Database::SQLite::Plugin::createDriver() const throw(const Fw::Exception&)
{
    return new Driver("Fw::Database::PostgreSQL::Plugin");
}

void Fw::Database::SQLite::Plugin::deleteDriver(Fw::Database::Driver* driver) const
{
    delete driver;
    driver = 0;
}

Fw::Database::SQLite::Plugin::Plugin(QObject* parent) :
    BaseClass(parent)
{
}

Fw::Database::SQLite::Plugin::~Plugin()
{
}

Q_EXPORT_PLUGIN2(sqliteImpl, Fw::Database::SQLite::Plugin)
