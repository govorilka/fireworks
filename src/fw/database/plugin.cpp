#include "QtCore/qdebug.h"

#include "fw/database/driver.hpp"
#include "fw/database/plugin.hpp"
#include "fw/helpers/filesystem.hpp"
#include "fwcore/fwml.h"

Fw::Database::PluginInterface::PluginInterface(QObject* parent) :
BaseClass(parent)
{

}

Fw::Database::PluginInterface::~PluginInterface()
{

}

////////////////////////////////////////////////////////////////////////////////

void Fw::Database::DriverLoader::DeleteLater(DriverLoader* driverLoader)
{
    if(driverLoader)
    {
        driverLoader->deleteLater();
        driverLoader = 0;
    }
}

Fw::Database::DriverLoader::DriverLoader(QObject* parent) :
    BaseClass(parent),
    m_driver(0)
{
}

Fw::Database::DriverLoader::~DriverLoader()
{
    unload();
}

bool Fw::Database::DriverLoader::load(FwMLObject* config) throw(const Fw::Exception&)
{
    const QByteArray driverPath = config->stringValue("path");
    if(!driverPath.isEmpty())
    {
        load(driverPath);
        return true;
    }

    const QByteArray driverName = config->stringValue("name");
    if(!driverName.isEmpty())
    {
        load( Fw::Helpers::FileSystem::pluginFileName(driverName) );
        return true;
    }

    throw Fw::Exception("Driver name and path is not defined");
    return false;
}

bool Fw::Database::DriverLoader::load(const QString& filename) throw(const Fw::Exception&)
{
    unload();

    setFileName(filename);
    if(!BaseClass::load())
    {
         throw Fw::Exception(QString("Can`t load plugin %1: %2").arg(filename).arg(errorString()).toUtf8());
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

    m_driver = plugin->createDriver();
}

bool Fw::Database::DriverLoader::unload()
{

    if(m_driver)
    {
        m_driver->close();

        if(QObject* const pluginObject = instance())
        {
            if(PluginInterface* plugin = qobject_cast<PluginInterface*>(pluginObject))
            {
                plugin->deleteDriver(m_driver);

            }
        }
        m_driver = 0;
    }

    return BaseClass::unload();
}

Fw::Database::Driver* Fw::Database::DriverLoader::driver() const throw(const Fw::Exception&)
{
    if(!m_driver)
    {
        throw Fw::Exception(m_driver->lastError());
    }
    return m_driver;
}
