#ifndef PLUGINIMPL_HPP
#define PLUGINIMPL_HPP

#include <plugin.hpp>

class PLUGINSHARED_EXPORT ResourceImpl : public ResourceInterface
{
public:
    explicit ResourceImpl();
    virtual ~ResourceImpl();

    virtual QString GetName() const;

private:
    static int s_counter;
    mutable int m_counter;
};

class PLUGINSHARED_EXPORT PluginImpl : public PluginInterface
{
    Q_OBJECT
    typedef PluginInterface BaseClass;

public:
    explicit PluginImpl(QObject* parent = 0);
    virtual ~PluginImpl();

    virtual ResourceInterface* instance() const throw(const Fw::Exception&);
    virtual void release(ResourceInterface* instance) const;

private:
    static int s_counter;
    mutable int m_counter;

    QString m_name;
};

#endif // PLUGINIMPL_HPP
