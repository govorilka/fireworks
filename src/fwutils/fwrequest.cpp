#include <QtCore/qcoreapplication.h>

#include "fwrequest.h"

FwRequestAnswer::FwRequestAnswer(int modalResult, const QByteArray& buttonname, QString caption, Qt::Key n_key) :
    m_modalResult(modalResult),
    m_caption(caption),
    m_key(n_key),
    m_name(buttonname)
{
}

//////////////////////////////////////////////////////////

FwRequest::FwRequest(QObject* sender) :
    m_sender(sender)
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

void FwRequest::postRequest(QObject* receiver)
{
    QCoreApplication::postEvent(receiver, new FwPostRequestEvent(*this));
}

int FwRequest::result(int key)
{
    foreach(const FwRequestAnswer& answerItem, m_answers)
    {
        if(answerItem.key() == key)
        {
            return answerItem.result();
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

