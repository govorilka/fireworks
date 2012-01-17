#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <QtCore/qobject.h>
#include <QtCore/qpluginloader.h>

#include "fw/exception.hpp"

#include "defs.hpp"
#include "plugin_global.hpp"

class PLUGINSHARED_EXPORT ResourceInterface
{
public:
    virtual ~ResourceInterface();

    virtual QString GetName() const = 0;
};

class PLUGINSHARED_EXPORT PluginInterface : public QObject
{
    Q_OBJECT
    typedef QObject BaseClass;

public:
    virtual ResourceInterface* instance() const throw(const Fw::Exception&) = 0;
    virtual void release(ResourceInterface* instance) const = 0;

protected:

    explicit PluginInterface(QObject* parent = 0);
    virtual ~PluginInterface();
};

class PLUGINSHARED_EXPORT PluginLoader
{
public:
    explicit PluginLoader(const QString& filename) throw(const Fw::Exception&);
    virtual ~PluginLoader();

    inline const PluginInterface& operator*() const throw(const Fw::Exception&);
    inline PluginInterface& operator*() throw(const Fw::Exception&);

    inline const PluginInterface* operator->() const throw(const Fw::Exception&);
    inline PluginInterface* operator->() throw(const Fw::Exception&);

private:
    QPluginLoader m_loader;

    PluginInterface* m_plugin;

};

const PluginInterface& PluginLoader::operator*() const throw(const Fw::Exception&)
{
    return *m_plugin;
}

PluginInterface& PluginLoader::operator*() throw(const Fw::Exception&)
{
    return *m_plugin;
}

const PluginInterface* PluginLoader::operator->() const throw(const Fw::Exception&)
{
    return m_plugin;
}

PluginInterface* PluginLoader::operator->() throw(const Fw::Exception&)
{
    return m_plugin;
}

#endif // PLUGIN_HPP
