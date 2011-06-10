#include <QtCore/qcoreapplication.h>
#include <QtCore/qdir.h>
#include <QtCore/qfileinfo.h>

#include "fwconfig.h"

#include "fwcore/fwml.h"
#include "fwcore/fwcppobject.h"

FwConfig::FwConfig()
{
}

QString FwConfig::configPath()
{
    Q_ASSERT(QCoreApplication::instance());
    QString path = QCoreApplication::applicationDirPath() + "/resources/config/";
    return QDir::toNativeSeparators(path + QFileInfo(QCoreApplication::applicationFilePath()).baseName());
}

QString FwConfig::configFilePath(const QString &fileName)
{
    QString path = configPath();
    if(!path.isEmpty())
    {
        return path += "/" + fileName;
    }
    return QString();
}

bool FwConfig::loadConfig(const QString& name, FwCPPObject* object)
{
    QString path = configFilePath(name + ".fwml");
    if(!path.isEmpty())
    {
        QFile fwmlConfig(path);

        FwMLObject rootObject;
        if(rootObject.parse(&fwmlConfig))
        {
            object->apply(&rootObject);
            return true;
        }
    }

    return false;
}
