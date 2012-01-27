#include <QtCore/qcoreapplication.h>
#include <QtCore/qdebug.h>
#include <QtCore/qpluginloader.h>
#include "fw/helpers/filesystem.hpp"

#include "plugin.hpp"

int main(int argc, char **argv)
{
    QCoreApplication a(argc, argv);

    QString path = Fw::Helpers::FileSystem::pluginsDir();
    qDebug() << path;

    QString pluginName1 = Fw::Helpers::FileSystem::pluginFileName("pluginimpl");
    qDebug() << pluginName1;

////////////////////////////////////////////////////////////////

    PluginLoader pluginLoader_1;
    PluginLoader pluginLoader_2;

    pluginLoader_1.load(pluginName1);
    pluginLoader_2.load(pluginName1);

    ResourceInterface* res_1 = pluginLoader_1.resource();
    ResourceInterface* res_2 = pluginLoader_1.resource();
    ResourceInterface* res_3 = pluginLoader_2.resource();
    ResourceInterface* res_4 = pluginLoader_2.resource();

    qDebug() << res_1->GetName();
    qDebug() << res_2->GetName();
    qDebug() << res_3->GetName();
    qDebug() << res_4->GetName();

    return 0;
}
