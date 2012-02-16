
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

Fw::JSON::RPC::Sentence::Sentence() :
    m_object(new Fw::JSON::Object())
{
}

////////////////////////////////////////////////////////////////////////////////

Fw::JSON::RPC::Request::Request(int id, const QString& method)
{
    m_object->addString("jsonrpc", "2.0");
    m_object->addNumber("id", id);
    m_object->addString("method", method);
    m_object->addObject("params");
}

bool Fw::JSON::RPC::Request::validation(ObjectPointer& object) const
{
    Q_UNUSED(object);
    return false;
}

////////////////////////////////////////////////////////////////////////////////

Fw::JSON::RPC::Response::Response()
{
}

bool Fw::JSON::RPC::Response::validation(ObjectPointer& object) const
{
    Q_UNUSED(object);
    return false;
}
