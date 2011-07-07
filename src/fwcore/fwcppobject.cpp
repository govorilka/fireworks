#include <QtCore/qcoreapplication.h>

#include <fwcore/fwml.h>

#include "fwcppobject.h"

FwCPPObject::FwCPPObject(const QByteArray& name) :
    m_name(name)
{
}

void FwCPPObject::apply(FwMLObject *object)
{
    Q_UNUSED(object);
}

bool FwCPPObject::loadData(const QString& fileName)
{
    QString path = QDir::toNativeSeparators(fileName);
    if(!QFile::exists(path))
    {
        path = QDir::toNativeSeparators(QCoreApplication::applicationDirPath() + "/resources/") + path;
        if(!QFile::exists(path))
        {
            qWarning(qPrintable(QString("File %1 not found").arg(path)));
            return false;
        }
    }

    QString error;
    FwMLObject rootObject;
    QFile fwmlData(path);
    if(!rootObject.parse(&fwmlData, &error))
    {
        qWarning(qPrintable(QString("FwML error in %1 file: %2").arg(path).arg(error)));
        return false;
    }

    apply(&rootObject);
    return true;
}

