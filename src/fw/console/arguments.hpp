#ifndef FIREWORKS_CONSOLE_ARGUMENTS_HPP
#define FIREWORKS_CONSOLE_ARGUMENTS_HPP

#include <QtCore/qbytearray.h>
#include <QtCore/qstringlist.h>

#include "defs.hpp"
#include "fwcore/fwcppobject.h"

class Fw::Console::Arguments
{
public:
    explicit Arguments(const QStringList& argList);
    ~Arguments();

    QString command() const;
    QStringList commandParams() const;

    QString value(const QString& key, const QString& defaultValue = QString()) const;

    inline static bool isKey(const QString& key);
    inline static bool isValue(const QString& value);
    static QString getClearArgument(const QString& arg);

private:
    QStringList m_argList;
};

bool Fw::Console::Arguments::isKey(const QString& key)
{
    return !key.isEmpty() && (key.startsWith("--") || key.startsWith('/'));
}

bool Fw::Console::Arguments::isValue(const QString& value)
{
    return !value.isEmpty() && !value.startsWith("--") && !value.startsWith('/');
}

#endif //FIREWORKS_CONSOLE_ARGUMENTS_HPP
