#include <QtCore/qfile.h>
#include <QtCore/qdir.h>

#include "fwmlengine.h"
#include "fwmldocument.h"

FwMLEngine::FwMLEngine(QObject *parent) :
    BaseClass(parent)
{
}

FwMLEngine::~FwMLEngine()
{
    foreach(FwMLDocument* document, m_documents)
    {
        delete document;
    }
    m_documents.clear();
}

FwMLDocument* FwMLEngine::addDocument(const QString& fileName)
{
    FwMLDocument* document = m_documents.value(fileName, 0);
    if(document)
    {
        return document;
    }

    document = new FwMLDocument(this);
    m_documents.insert(fileName, document);

    QString path = QDir::toNativeSeparators(fileName);
    if(!QFile::exists(path) && !m_rootDirectory.isEmpty())
    {
        path = QDir(m_rootDirectory).absoluteFilePath(fileName);
    }

    QFile documentData(path);
    if(!QFile::exists(path))
    {
        qWarning(qPrintable(QString("File %1 not found").arg(path)));
        return 0;
    }

    document->load(&documentData);
    return document;
}
