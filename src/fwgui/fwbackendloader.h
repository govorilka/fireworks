#ifndef FIREWORKS_BACKENDLOADER_H
#define FIREWORKS_BACKENDLOADER_H

#include <QtCore/qpluginloader.h>

#include "fireworks.h"

class FwGraphicsView;

class FIREWORKSSHARED_EXPORT FwBackendLoader : public QPluginLoader
{
    Q_OBJECT
    typedef QPluginLoader BaseClass;

public:
    explicit FwBackendLoader(QObject *parent = 0);

    FwGraphicsView* loadView(const QString& fileName);
};

#endif // FWBACKENDLOADER_H
