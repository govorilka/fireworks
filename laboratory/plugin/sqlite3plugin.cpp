#include <QtCore/qpluginloader.h>
#include <QtCore/qplugin.h>.

#include "sqlite3plugin.hpp"

Sqlite3Plugin::Sqlite3Plugin(QObject *parent) :
    BaseClass(parent)
{
}

Sqlite3Plugin::~Sqlite3Plugin()
{
}

QString Sqlite3Plugin::hi() const
{
    return "Hi from SQLite3 plugins!!!";
}

Q_EXPORT_PLUGIN2(sqlite3, Sqlite3Plugin)
