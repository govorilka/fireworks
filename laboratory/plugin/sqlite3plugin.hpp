#ifndef SQLITE3PLUGIN_HPP
#define SQLITE3PLUGIN_HPP

#include "plugin_global.hpp"

#include "fw/database/driver.hpp"

class PLUGINSHARED_EXPORT Sqlite3Plugin : public Fw::Database::Plugin
{
    Q_OBJECT
    typedef Fw::Database::Plugin BaseClass;

public:
    explicit Sqlite3Plugin(QObject *parent = 0);
    virtual ~Sqlite3Plugin();

    virtual QString hi() const;
};

#endif // SQLITE3PLUGIN_HPP
