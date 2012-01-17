#ifndef FIREWORKS_CONSOLE_ARGUMENTS_HPP
#define FIREWORKS_CONSOLE_ARGUMENTS_HPP

#include <QtCore/qbytearray.h>
#include <QtCore/qstringlist.h>

#include "defs.hpp"
#include "fwcore/fwcppobject.h"

namespace Fw
{
    namespace Console
    {
        struct Argument;
    }
}

struct FIREWORKSSHARED_EXPORT Fw::Console::Argument
{
    QString name;
    QString value;

    inline bool isEmpty() const;
    inline bool operator==(const Argument& other) const;
    inline bool operator!=(const Argument& other) const;
};

class FIREWORKSSHARED_EXPORT Fw::Console::Arguments
{
public:

    explicit Arguments(const QStringList& argList);
    ~Arguments();

    bool isExist(const QString& key, QString* value = 0) const;
    const QString value(const QString& key, const QString& defaultValue = "") const;

    inline const QList<Argument>& toList() const;

private:
    inline static bool isKey(const QString& key);
    inline static bool isValue(const QString& value);
    static QString getClearArgument(const QString& arg);

    QList<Argument> m_arguments;
};

const QList<Fw::Console::Argument>& Fw::Console::Arguments::toList() const
{
    return m_arguments;
}

bool Fw::Console::Arguments::isKey(const QString& key)
{
    return !key.isEmpty() && key.startsWith("--");
}

bool Fw::Console::Arguments::isValue(const QString& value)
{
    return !value.isEmpty() && !value.startsWith("--");
}

///////////////////////////////////////////////////////////////////////////////

bool Fw::Console::Argument::isEmpty() const
{
    return name.isEmpty() && value.isEmpty();
}

bool Fw::Console::Argument::operator==(const Argument& other) const
{
    return name == other.name && value == other.value;
}

bool Fw::Console::Argument::operator!=(const Argument& other) const
{
    return name != other.name || value != other.value;
}

#endif //FIREWORKS_CONSOLE_ARGUMENTS_HPP
