#ifndef FWMLDOCUMENT_H
#define FWMLDOCUMENT_H

#include <QtCore/qobject.h>
#include <QtCore/qdebug.h>

#include "fwcore/fwml.h"

#include "fw/exception.hpp"

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

    void load(QIODevice* device) throw(Fw::Exception&);

    FwMLObject* findObject(const QByteArray& objectName);

    inline static QByteArray objectName(const QByteArray& className,
                                        int numberSignIndex);

protected:

    void bindingClassTravel(FwMLObject* object);
    void bindingClass(FwMLObject* object);

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

QByteArray FwMLDocument::objectName(const QByteArray& className,
                                    int numberSignIndex)
{
    if(numberSignIndex != -1)
    {
        return className.right(className.length() - numberSignIndex - 1);
    }
    return className;
}

#endif // FWMLDOCUMENT_H
