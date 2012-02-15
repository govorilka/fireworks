
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
        response.parse(reply);
    }
    catch(const Fw::Exception& e)
    {
        response.setNetworkError(e.error());
    }
    reply->deleteLater();
    emit finished(response);

}

////////////////////////////////////////////////////////////////////////////////

Fw::JSON::RPC::Request::Request(int id, const QString& method)
{
    m_object->addString("jsonrpc", "2.0");
    m_object->addNumber("id", id);
    m_object->addString("method", method);
    m_object->addObject("params");
}

////////////////////////////////////////////////////////////////////////////////

Fw::JSON::RPC::Response::Response()
{
}

int Fw::JSON::RPC::Response::errorCode() const
{
    if(const Fw::JSON::Object* errorObject = error())
    {
        return errorObject->numberAttribute("code");
    }

    return 0;
}

QString Fw::JSON::RPC::Response::errorMessage() const
{
    if(const Fw::JSON::Object* errorObject = error())
    {
        return errorObject->stringAttribute("message");
    }

    return QString();
}

const Fw::JSON::Object* const Fw::JSON::RPC::Response::errorData() const
{
    if(const Fw::JSON::Object* errorObject = error())
    {
        return errorObject->objectAttribute("data");
    }

    return 0;
}
