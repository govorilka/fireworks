#ifndef PLUGIN_INL_HPP
#define PLUGIN_INL_HPP

#include "plugin.hpp"

Fw::Database::Driver* PluginLoader::resource() const
{
    return m_driver;
}

//PluginLoader::PluginLoader(QObject* parent) :
//    BaseClass(parent),
//    m_resource(0)
//{
//}

//PluginLoader::~PluginLoader()
//{
//    unload();
//}

//bool PluginLoader::load(const QString& filename) throw(const Fw::Exception&)
//{
//    unload();
//    setFileName(filename);
//    if(!BaseClass::load())
//    {
//        throw Fw::Exception(QString("PluginLoader::PluginLoader : Can`t load plugin - ") + filename);
//    }

//    QObject* const pluginObject = instance();
//    if(!pluginObject)
//    {
//        throw Fw::Exception(QString("PluginLoader::PluginLoader : Can`t create instance of plugin -") + filename);
//    }

//    PluginInterface* plugin = qobject_cast<PluginInterface*>(pluginObject);
//    if(!plugin)
//    {
//        throw Fw::Exception(QString("PluginLoader::PluginLoader : Plugin have wrong type -") + filename);
//    }

//    m_resource = plugin->instance();
//}

//bool PluginLoader::unload()
//{
//    if(m_resource)
//    {
//        if(QObject* const pluginObject = instance())
//        {
//            if(PluginInterface* plugin = qobject_cast<PluginInterface*>(pluginObject))
//            {
//                plugin->release(m_resource);
//                m_resource = 0;
//            }
//        }
//    }

//    return BaseClass::unload();
//}




#endif // PLUGIN_INL_HPP
