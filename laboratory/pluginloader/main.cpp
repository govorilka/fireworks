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

    PluginLoader pluginLoader_1(pluginName1);
    PluginLoader pluginLoader_2(pluginName1);

    ResourceInterface* res_1 = pluginLoader_1->instance();
    ResourceInterface* res_2 = pluginLoader_1->instance();
    ResourceInterface* res_3 = pluginLoader_2->instance();
    ResourceInterface* res_4 = pluginLoader_2->instance();

    qDebug() << res_1->GetName();
    qDebug() << res_2->GetName();
    qDebug() << res_3->GetName();
    qDebug() << res_4->GetName();

    pluginLoader_1->release(res_1);
    pluginLoader_1->release(res_2);
    pluginLoader_2->release(res_3);
    pluginLoader_2->release(res_4);


    return 0;
}
