#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <QtCore/qobject.h>

#include "plugin_global.hpp"


class PLUGINSHARED_EXPORT Plugin : public QObject
{
    Q_OBJECT
    typedef QObject BaseClass;

public:
    Plugin(QObject* parent = 0);

    virtual QString hi() const = 0;
};

#endif // PLUGIN_HPP
