#include <QtCore/qiodevice.h>

#include "fwmldocument.h"
#include "fwmlengine.h"
#include "fwml.h"

FwMLDocument::FwMLDocument(FwMLEngine* engine) :
    BaseClass(engine),
    m_engine(engine),
    m_rootObject(new FwMLObject())
{
}

bool FwMLDocument::load(QIODevice* device)
{
    m_errorString = QString();
    m_rootObject->removeAttributes();
    return m_rootObject->parse(device, &m_errorString);
}
