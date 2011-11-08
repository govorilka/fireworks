#include <QtCore/qiodevice.h>
#include <QtCore/qdebug.h>

#include "fwmldocument.h"
#include "fwmlengine.h"
#include "fwml.h"

FwMLDocument::FwMLDocument(FwMLEngine* engine) :
    BaseClass(engine),
    m_engine(engine),
    m_rootObject(new FwMLObject())
{
}

void FwMLDocument::load(QIODevice* device) throw(Fw::Exception&)
{
    m_errorString = QString();
    m_rootObject->removeAttributes();
    m_rootObject->parse(device);
    bindingClassTravel(m_rootObject);
}

void FwMLDocument::bindingClassTravel(FwMLObject* object)
{
    bindingClass(object);
    foreach(FwMLNode* node, object->attributes())
    {
        FwMLObject* childObject = node->cast<FwMLObject>();
        if(childObject)
        {
            bindingClassTravel(childObject);
        }
    }
}

void FwMLDocument::bindingClass(FwMLObject* object)
{
    QByteArray className = object->className();
    if(!className.isEmpty())
    {
        FwMLDocument* document = this;

        int signIndex = className.indexOf('#');
        if(signIndex > 0)
        {
            if(!m_engine)
            {
                m_errorString = tr("Cannot sets FwMLEngine pointer");
                return;
            }

            QByteArray documentName = className.left(signIndex);
            if(!(document = m_engine->addDocument(documentName, this)))
            {
                m_errorString = tr("Document %1 is not exists")
                                    .arg(QString::fromUtf8(documentName));
                return;
            }
        }

        FwMLObject* classObject = document->findObject(objectName(className, signIndex));
        if(!classObject)
        {
            m_errorString = tr("Object %1 is not exists")
                                .arg(QString::fromUtf8(className));
            return;
        }
        object->setClassObject(classObject);
    }
}

FwMLObject* FwMLDocument::findObject(const QByteArray& objectName)
{
    if(objectName.isEmpty() || objectName == "root")
    {
        return m_rootObject;
    }
    return 0;
}
