#include <QtCore/qpluginloader.h>
#include <QtCore/qplugin.h>.

#include "pluginimpl.hpp"

PluginImpl::PluginImpl(QObject* parent) :
    BaseClass(parent)
{
}

PluginImpl::~PluginImpl()
{
}

QString PluginImpl::hi() const
{
    return "Hi from PluginImpl!!!";
}

Q_EXPORT_PLUGIN2(pluginimpl, PluginImpl)
