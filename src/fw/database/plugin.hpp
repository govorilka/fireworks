#ifndef FIREWORKS_PLUGIN_HPP
#define FIREWORKS_PLUGIN_HPP

#include <QtCore/qpluginloader.h>

#include "fw/exception.hpp"
#include "fw/database/defs.hpp"

class FIREWORKSSHARED_EXPORT Fw::Database::PluginInterface : public QObject
{
    Q_OBJECT
    typedef QObject BaseClass;

public:
    virtual Fw::Database::Driver* createDriver() const throw(const Fw::Exception&) = 0;
    virtual void deleteDriver(Fw::Database::Driver* instance) const = 0;

protected:
    explicit PluginInterface(QObject* parent = 0);
    virtual ~PluginInterface();
};

////////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT Fw::Database::DriverLoader : public QPluginLoader
{
    Q_OBJECT
    typedef QPluginLoader BaseClass;

public:
    static void DeleteLater(DriverLoader* driverLoader);

    explicit DriverLoader(QObject* parent = 0);
    virtual ~DriverLoader();

    bool load(FwMLObject* config) throw(const Fw::Exception&);
    bool load(const QString& filename) throw(const Fw::Exception&);
    bool unload();

    Fw::Database::Driver* driver() const throw(const Fw::Exception&);

private:
    Fw::Database::Driver* m_driver;
};

#endif // FIREWORKS_PLUGIN_HPP
