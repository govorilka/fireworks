#include <QtCore/qpluginloader.h>
#include <QtCore/qplugin.h>
#include <QtCore/qdebug.h>.

#include "pluginimpl.hpp"

int ResourceImpl::s_counter = 0;

ResourceImpl::ResourceImpl() :
m_counter(0)
{
    qDebug() << "ResourceImpl()";
}

ResourceImpl::~ResourceImpl()
{
    qDebug() << "~ResourceImpl()";
}

QString ResourceImpl::GetName() const
{
    return QString("ResourceImpl::GetName() : s_counter(%1), m_counter(%2)").arg(s_counter++).arg(m_counter++);
}

////////////////////////////////////////////////////////////////////////////////

int PluginImpl::s_counter = 0;

PluginImpl::PluginImpl(QObject* parent) :
    BaseClass(parent),
    m_counter(0),
    m_name("PluginImpl_1")
{
    qDebug() << QString("PluginImpl(): m_name(%0)").arg(m_name);
}

PluginImpl::~PluginImpl()
{
    qDebug() << QString("~PluginImpl(): m_name(%0)").arg(m_name);
}

ResourceInterface* PluginImpl::instance() const throw(const Fw::Exception&)
{
    return new ResourceImpl;
}

void PluginImpl::release(ResourceInterface* instance) const
{
    if(instance)
    {
        delete instance;
    }
}

Q_EXPORT_PLUGIN2(pluginimpl, PluginImpl)
