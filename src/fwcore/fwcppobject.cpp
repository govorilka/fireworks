#include <QtCore/qcoreapplication.h>

#include <fwcore/fwml.h>

#include "fwcppobject.h"

FwCPPObject::FwCPPObject(const QByteArray& name) :
    m_name(name)
{

}

bool FwCPPObject::loadFile(const QString& fileName)
{
    QFile fwmlData(QDir::toNativeSeparators(fileName));
    if(!fwmlData.exists())
    {
        qWarning(qPrintable(QString("File %1 not found").arg(fileName)));
        return false;
    }

    QString error;
    FwMLObject rootObject;
    if(!rootObject.parse(&fwmlData, &error))
    {
        qWarning(qPrintable(QString("FwML error in %1 file: %2").arg(fileName).arg(error)));
        return false;
    }

    loadData(&rootObject);
    return true;
}

void FwCPPObject::resetData()
{
}
