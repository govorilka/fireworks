#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <QtCore/qobject.h>
#include <QtCore/qpluginloader.h>

#include "fw/exception.hpp"
#include "fw/database/driver.hpp"

#include "defs.hpp"
#include "plugin_global.hpp"

class PLUGINSHARED_EXPORT PluginInterface : public QObject
{
    Q_OBJECT
    typedef QObject BaseClass;

public:
    virtual Fw::Database::Driver* instance() const throw(const Fw::Exception&) = 0;
    virtual void release(Fw::Database::Driver* instance) const = 0;

protected:
    explicit PluginInterface(QObject* parent = 0);
    virtual ~PluginInterface();
};

class PLUGINSHARED_EXPORT PluginLoader : public QPluginLoader
{
    Q_OBJECT
    typedef QPluginLoader BaseClass;

public:
    explicit PluginLoader(QObject* parent = 0);
    virtual ~PluginLoader();

    bool load(const QString& filename) throw(const Fw::Exception&);
    bool unload();

    inline Fw::Database::Driver* resource() const;

private:
    Fw::Database::Driver* m_driver;
};

#include "plugin_inl.hpp"

#endif // PLUGIN_HPP
