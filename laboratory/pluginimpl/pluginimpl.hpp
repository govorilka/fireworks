#ifndef PLUGINIMPL_HPP
#define PLUGINIMPL_HPP

#include <plugin.hpp>

class PLUGINSHARED_EXPORT PluginImpl : public Plugin
{
    Q_OBJECT
    typedef Plugin BaseClass;

public:
    explicit PluginImpl(QObject* parent = 0);
    virtual ~PluginImpl();

    virtual QString hi() const;
};

#endif // PLUGINIMPL_HPP
