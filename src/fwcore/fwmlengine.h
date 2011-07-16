#ifndef FIREWORKS_MLENGINE_H
#define FIREWORKS_MLENGINE_H

#include <QtCore/qobject.h>
#include <QtCore/qhash.h>

#include "fwcore/fwml.h"

class FwMLDocument;

class FIREWORKSSHARED_EXPORT FwMLEngine : public QObject
{
    Q_OBJECT
    typedef QObject BaseClass;

public:
    explicit FwMLEngine(QObject *parent = 0);
    ~FwMLEngine();

    inline QHash<QString, FwMLDocument*> documents() const;

    FwMLDocument* addDocument(const QString& fileName);
    FwMLDocument* addDocument(const QString &fileName, FwMLDocument* currentDocument);

    inline QString rootDirectory() const;
    inline void setRootDirectory(const QString& path);

private:
    QHash<QString, FwMLDocument*> m_documents;
    QString m_rootDirectory;
};

QHash<QString, FwMLDocument*> FwMLEngine::documents() const
{
    return m_documents;
}

QString FwMLEngine::rootDirectory() const
{
    return m_rootDirectory;
}

void FwMLEngine::setRootDirectory(const QString& path)
{
    m_rootDirectory = path;
}

#endif // FIREWORKS_MLENGINE_H
