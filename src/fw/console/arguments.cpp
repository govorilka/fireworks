#include "arguments.hpp"

Fw::Console::Arguments::Arguments(const QStringList& argList) :
    m_arguments()
{
    QStringList::const_iterator iter = argList.begin() + 1;
    QStringList::const_iterator end = argList.end();

    Argument argument;
    for(; iter != end; ++iter)
    {
        const QString& candidate = *iter;
        if(isKey(candidate))
        {
            if(!argument.name.isEmpty())
            {
                m_arguments.push_back(argument);
                argument.value.clear();
            }
            argument.name = Arguments::getClearArgument(candidate);
        }
        else if(isValue(candidate))
        {
        argument.value = Arguments::getClearArgument(candidate);
        m_arguments.push_back(argument);
        argument.name.clear();
        argument.value.clear();
        }
    }

    if(!argument.isEmpty() && (m_arguments.isEmpty() || m_arguments.back() != argument))
    {
        m_arguments.push_back(argument);
    }
}

Fw::Console::Arguments::~Arguments()
{

}

QString Fw::Console::Arguments::value(const QString& key, const QString& defaultValue) const
{
    foreach(Argument argument, m_arguments)
    {
        if(argument.name == key)
        {
            return argument.value;
        }
    }

    return defaultValue;
}

bool Fw::Console::Arguments::isExist(const QString& key, QString* value) const
{
    foreach(Argument argument, m_arguments)
    {
        if(argument.name == key)
        {
            if(value)
            {
                (*value) = argument.value;
            }
            return true;
        }
    }

    return false;
}

QString Fw::Console::Arguments::getClearArgument(const QString& arg)
{
    if(arg.startsWith('"') && arg.endsWith('"'))
    {
        return arg.mid(1, arg.size()-1);
    }
    else if(arg.startsWith('/'))
    {
        return arg.mid(1);
    }
    else if(arg.startsWith("--"))
    {
        return arg.mid(2);
    }

    return arg;
}
