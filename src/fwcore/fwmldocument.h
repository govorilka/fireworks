#ifndef FWMLDOCUMENT_H
#define FWMLDOCUMENT_H

#include <QtCore/qobject.h>

#include "fwcore/fwml.h"

class QIODevice;

class FwMLEngine;
class FwMLObject;

class FIREWORKSSHARED_EXPORT FwMLDocument : public QObject
{
    Q_OBJECT
    typedef QObject BaseClass;

public:
    explicit FwMLDocument(FwMLEngine* engine = 0);

    inline FwMLEngine* engine() const;
    inline FwMLObject* rootObject() const;

    inline QString errorString() const;

    bool load(QIODevice* device);

private:
    FwMLEngine* m_engine;
    FwMLObject* m_rootObject;
    QString m_errorString;
};

FwMLEngine* FwMLDocument::engine() const
{
    return m_engine;
}

FwMLObject* FwMLDocument::rootObject() const
{
    return m_rootObject;
}

QString FwMLDocument::errorString() const
{
    return m_errorString;
}

#endif // FWMLDOCUMENT_H
