#ifndef FW_DATABASE_POSTGRESQL_PLUGIN_HPP
#define FW_DATABASE_POSTGRESQL_PLUGIN_HPP

#include "fw/database/plugin.hpp"

#include "plugins/postgresql/defs.hpp"

class Fw::Database::PostgreSQL::Plugin : public Fw::Database::PluginInterface
{
    Q_OBJECT
    typedef PluginInterface BaseClass;

public:
    explicit Plugin(QObject* parent = 0);
    virtual ~Plugin();

    virtual Fw::Database::Driver* createDriver() const throw(const Fw::Exception&);
    virtual void deleteDriver(Fw::Database::Driver* driver) const;
};

#endif // FW_DATABASE_POSTGRESQL_PLUGIN_HPP
