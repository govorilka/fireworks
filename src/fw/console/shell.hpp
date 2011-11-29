#ifndef FIREWORKS_CONSOLE_SHELL_HPP
#define FIREWORKS_CONSOLE_SHELL_HPP

#include <QtCore/QMap>

#include "defs.hpp"
#include "fwcore/fwcppobject.h"

template<class T> class Fw::Console::Shell : public FwCPPObject
{
    typedef FwCPPObject BaseClass;

public:
    Shell(T& receiver, const QByteArray& name = "Arguments");

    typedef void (T::*Method)(const QString& param);

    void addCommand(const QString& command, Method callback);
    void execute(const QString& command, const QString& argument) throw(Fw::Exception&);

    virtual bool loadData(FwMLObject* object);

private:
    T& m_receiver;
    typedef QMap<QString, Method> Commands;
    Commands m_commands;
};

template <class T>
Fw::Console::Shell<T>::Shell(T& receiver, const QByteArray& name) :
    BaseClass(name),
    m_receiver(receiver)
{
}

template <class T>
void Fw::Console::Shell<T>::addCommand(const QString& command, Method callback)
{
    m_commands.insert(command, callback);
}

template <class T>
void Fw::Console::Shell<T>::execute(const QString& command, const QString& argument) throw(Fw::Exception&)
{
    Method method = m_commands.value(command, 0);
    if(method)
    {
        (m_receiver.*method)(argument);
    }
}

template <class T>
bool Fw::Console::Shell<T>::loadData(FwMLObject* object)
{
    return true;
}

#endif // FIREWORKS_CONSOLE_SHELL_HPP
