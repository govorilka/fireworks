#ifndef FW_DATABASE_SQLITE_PLUGIN_HPP
#define FW_DATABASE_SQLITE_PLUGIN_HPP

#include "fw/database/plugin.hpp"

#include "plugins/sqlite/defs.hpp"

class Fw::Database::SQLite::Plugin : public Fw::Database::PluginInterface
{
    Q_OBJECT
    typedef PluginInterface BaseClass;

public:
    virtual Fw::Database::Driver* createDriver() const throw(const Fw::Exception&) = 0;
    virtual void deleteDriver(Fw::Database::Driver* driver) const = 0;

protected:
    explicit Plugin(QObject* parent = 0);
    virtual ~Plugin();
};

#endif // FW_DATABASE_SQLITE_PLUGIN_HPP
