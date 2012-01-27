#include "QtCore/qdebug.h"

#include "plugin.hpp"


PluginInterface::PluginInterface(QObject* parent) :
BaseClass(parent)
{

}

PluginInterface::~PluginInterface()
{

}

////////////////////////////////////////////////////////////////////////////////

PluginLoader::PluginLoader(QObject* parent) :
    BaseClass(parent),
    m_driver(0)
{
}

PluginLoader::~PluginLoader()
{
    unload();
}

bool PluginLoader::load(const QString& filename) throw(const Fw::Exception&)
{
    unload();
    setFileName(filename);
    if(!BaseClass::load())
    {
        throw Fw::Exception(QString("PluginLoader::PluginLoader : Can`t load plugin - ") + filename);
    }

    QObject* const pluginObject = instance();
    if(!pluginObject)
    {
        throw Fw::Exception(QString("PluginLoader::PluginLoader : Can`t create instance of plugin -") + filename);
    }

    PluginInterface* plugin = qobject_cast<PluginInterface*>(pluginObject);
    if(!plugin)
    {
        throw Fw::Exception(QString("PluginLoader::PluginLoader : Plugin have wrong type -") + filename);
    }

    m_driver = plugin->instance();
}

bool PluginLoader::unload()
{
    if(m_driver)
    {
        if(QObject* const pluginObject = instance())
        {
            if(PluginInterface* plugin = qobject_cast<PluginInterface*>(pluginObject))
            {
                plugin->release(m_driver);

            }
        }
        m_driver = 0;
    }

    return BaseClass::unload();
}
