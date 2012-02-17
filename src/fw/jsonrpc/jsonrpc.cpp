
#include "fw/jsonrpc/jsonrpc.hpp"


Fw::JSON::RPC::RPC(QNetworkAccessManager* networkManager) :
    m_networkManager(networkManager)
{
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finish(QNetworkReply*)));
}

void Fw::JSON::RPC::send(const QUrl& serverURL, const Fw::JSON::RPC::Request& request)
{
    m_networkManager->post(QNetworkRequest(serverURL), request.toUtf8());
}

void Fw::JSON::RPC::finish(QNetworkReply* reply)
{
    Response response;
    try
    {
        if(reply->error() != QNetworkReply::NoError)
        {
            throw Fw::Exception("Server error:" + reply->errorString().toUtf8());
        }
        //response.parse(reply);
    }
    catch(const Fw::Exception& e)
    {
        // response.setNetworkError(e.error());
    }
    reply->deleteLater();
    emit finished(response);

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

void Fw::JSON::RPC::Sentence::parse(QIODevice* ioDevice) throw(const Fw::Exception&)
{
    m_object->parse(ioDevice);
    QByteArray errorMessage;
    if(!isValid(&errorMessage))
    {
        throw Fw::Exception(errorMessage);
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

        QString version = m_object->value<Fw::JSON::String>("jsonrpc");
        if(version != "2.0")
        {
            throw Fw::Exception("Unsupported version json rpc or attribute 'jsonprc' is not define."
                                "This library used version 2.0");
        }

        validation(m_object);
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
