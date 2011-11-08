#include "arguments.hpp"

Fw::Console::Arguments::Arguments(const QStringList& argList) :
    m_argList(argList)
{

}

Fw::Console::Arguments::~Arguments()
{

}

QString Fw::Console::Arguments::command() const
{
    if(m_argList.size() > 2 && isValue(m_argList.at(1)))
    {
        return m_argList.at(1);
    }

    return QString();
}

QStringList Fw::Console::Arguments::commandParams() const
{
    QStringList result;
    if(m_argList.size() >= 3)
    {
        QStringList::const_iterator iter = m_argList.begin() + 2;
        QStringList::const_iterator end = m_argList.end();
        for(; iter != end; ++iter)
        {
            const QString& candidate = *iter;
            if(isKey(candidate))
            {
                return result;
            }
            result.append(candidate);
        }
    }
    return result;
}

QString Fw::Console::Arguments::value(const QString& key, const QString& defaultValue) const
{
    //TODO: Compare key!!!
    if(m_argList.size() > 2)
    {
        QStringList::ConstIterator last = m_argList.end() - 1;
        for(QStringList::ConstIterator iter = m_argList.begin() + 1; iter != m_argList.end(); ++iter)
        {
            const QString& candidatKey = *iter;
            if(isKey(candidatKey))
            {
                if(iter != last)
                {
                    const QString& candidatValue = *(iter + 1);
                    if(!isKey(candidatValue))
                    {
                        return candidatValue;
                    }
                }
            }
        }
    }
    return defaultValue;
}

QString Fw::Console::Arguments::getClearArgument(const QString& arg)
{
    if(arg.startsWith('"') && arg.endsWith('"'))
    {

    }
    else if(arg.startsWith('/'))
    {

    }
    else if(arg.startsWith("--"))
    {

    }

    return arg;
}
