#include <QtCore/qdebug.h>

#include "fw/jsonrpc/jsonrpc.hpp"


Fw::JSON::RPC::RPC(QNetworkAccessManager* networkManager, QObject* parent) :
    BaseClass(parent),
    m_networkManager(networkManager)
{
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finish(QNetworkReply*)));
}

void Fw::JSON::RPC::send(const QUrl& serverURL, const Request& request) throw (const Fw::Exception&)
{
    request.valid();
    qDebug() << request.toUtf8();
    if(m_networkManager)
    {
        m_networkManager->post(QNetworkRequest(serverURL), request.toUtf8());
    }
}

void Fw::JSON::RPC::send(const QUrl& serverURL, const QString& request) throw (const Fw::Exception&)
{
    Request r;
    r.parse(request);
    send(serverURL, r);
}

void Fw::JSON::RPC::finish(QNetworkReply* reply)
{
    try
    {
        if(reply->error() != QNetworkReply::NoError)
        {
            throw Fw::Exception("Server error:" + reply->errorString().toUtf8());
        }

        Response response;
        response.parse(reply);
        emit finished(response);

    }
    catch(const Fw::Exception& e)
    {
        qDebug() << e.error();
        emit error(e.error());
    }
    reply->deleteLater();
}

////////////////////////////////////////////////////////////////////////////////

Fw::JSON::RPC::Sentence::Sentence(int id) :
    m_object(new Fw::JSON::Object())
{
    m_object->addString("jsonrpc", "2.0");
    if(id)
    {
        m_object->addNumber("id", id);
    }
}

bool Fw::JSON::RPC::Sentence::isValid(QByteArray* errorMessage) const
{
    try
    {
        if(!m_object->hasValue<Fw::JSON::Number>("id"))
        {
            throw Fw::Exception("Attribute 'id' is not define");
        }

        Fw::JSON::Node* version = m_object->attribute("jsonrpc");
        if(!version)
        {
            throw Fw::Exception("Attribute 'jsonprc' is not define."
                                "This library used version 2.0");
        }

        bool bOk = false;
        if(version->toUint(&bOk) != 2)
        {
            throw Fw::Exception("Unsupported version json rpc."
                                "This library used version 2.0");
        }

        valid();
    }
    catch(const Fw::Exception& e)
    {
        if(errorMessage)
        {
            (*errorMessage) = e.error();
        }
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////

Fw::JSON::RPC::Request::Request(int id, const QString& method, Fw::JSON::Node* params) :
    BaseClass(id)
{
    if(!method.isEmpty())
    {
        m_object->addString("method", method);
    }
    if(params)
    {
        m_object->addAttribute("params", params);
    }
}

void Fw::JSON::RPC::Request::validation(const ObjectPointer& object) const throw(const Fw::Exception&)
{
    Q_UNUSED(object);
}

////////////////////////////////////////////////////////////////////////////////

Fw::JSON::RPC::Response::Response(int id) :
    BaseClass(id)
{
}

void Fw::JSON::RPC::Response::validation(const ObjectPointer& object) const throw(const Fw::Exception&)
{
    Q_UNUSED(object);
}
