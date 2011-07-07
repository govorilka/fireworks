#include <QtCore/qcoreapplication.h>

#include "fwrequest.h"

FwRequestAnswer::FwRequestAnswer() :
    m_modalResult(0),
    m_key(0)
{
}

FwRequestAnswer::FwRequestAnswer(int modalResult, const QByteArray& name) :
    m_modalResult(modalResult),
    m_name(name),
    m_key(0)
{
}

//////////////////////////////////////////////////////////

FwRequest::FwRequest(QObject* sender, const QString& text) :
    m_sender(sender),
    m_text(text)
{
    m_uid = QUuid::createUuid();
}


void FwRequest::postAnswer(int result)
{
    if(m_sender)
    {
        QCoreApplication::postEvent(m_sender, new FwResult(m_uid, result));
    }
}

QUuid FwRequest::postRequest(QObject* receiver)
{
    QCoreApplication::postEvent(receiver, new FwPostRequestEvent(*this));
    return id();
}

int FwRequest::result(int key)
{
    foreach(FwRequestAnswer answer, m_answers)
    {
        if(answer.key() == key)
        {
            return answer.result();
        }
    }
    return 0;
}

//////////////////////////////////////////////////////////

FwPostRequestEvent::FwPostRequestEvent(const FwRequest& request) :
    BaseClass(static_cast<QEvent::Type>(typeID())),
    m_request(request)
{
}

int FwPostRequestEvent::typeID()
{
    static int _typeID = QEvent::registerEventType();
    return _typeID;
}

//////////////////////////////////////////////////////////

FwResult::FwResult(QUuid uid, int result) :
    BaseClass(static_cast<QEvent::Type>(typeID())),
    m_id(uid),
    m_result(result)
{
}

int FwResult::typeID()
{
    static int _typeID = QEvent::registerEventType();
    return _typeID;
}

