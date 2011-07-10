#include <QtCore/qfile.h>

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

    QFile documentData(fileName);
    if(documentData.exists())
    {
        document->load(&documentData);
    }

    return document;
}
