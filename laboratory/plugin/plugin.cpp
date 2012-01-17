#include "QtCore/qdebug.h"

#include "plugin.hpp"

ResourceInterface::~ResourceInterface()
{
}

////////////////////////////////////////////////////////////////////////////////

PluginInterface::PluginInterface(QObject* parent) :
BaseClass(parent)
{
}

PluginInterface::~PluginInterface()
{
}

////////////////////////////////////////////////////////////////////////////////

PluginLoader::PluginLoader(const QString& filename) throw(const Fw::Exception&) :
    m_loader(filename),
    m_plugin(0)
{
    qDebug() << "PluginLoader()";

    if(!m_loader.load())
    {
        throw Fw::Exception(QString("PluginLoader::PluginLoader : Can`t load plugin - ") + filename);
    }

    QObject* const plugin = m_loader.instance();
    if(!plugin)
    {
        throw Fw::Exception(QString("PluginLoader::PluginLoader : Can`t create instance of plugin -") + filename);
    }

    m_plugin = qobject_cast<PluginInterface*>(plugin);
    if(!m_plugin)
    {
        m_loader.unload();
        throw Fw::Exception(QString("PluginLoader::PluginLoader : Plugin have wrong type -") + filename);
    }
}

PluginLoader::~PluginLoader()
{
    m_loader.unload();
    m_plugin = 0;

    qDebug() << "~PluginLoader()";
}
