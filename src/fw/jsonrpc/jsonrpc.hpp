#ifndef FIREWORKS_JSONRPC_HPP
#define FIREWORKS_JSONRPC_HPP

#include <QtCore/QSharedPointer>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>

#include "fw/core/json.hpp"

namespace Fw
{
    namespace JSON
    {
        class RPC;
    }
}

////////////////////////////////////////////////////////////////////////////////

class Fw::JSON::RPC : public QObject
{
    Q_OBJECT
    typedef QObject BaseClass;

public:

    class Sentence;
    class Request;
    class Response;

    RPC(QNetworkAccessManager* networkManager);

    void send(const QUrl& serverURL, const Request& request);

signals:
    void finished(const Response& response);

private:
    QNetworkAccessManager* m_networkManager;

private slots:
    void finish(QNetworkReply* reply);

};

////////////////////////////////////////////////////////////////////////////////

class Fw::JSON::RPC::Sentence
{
public:
    Sentence();

    bool isValid() const;

    void parse(QIODevice* ioDevice) throw(Fw::Exception);

    inline int id() const;
    inline QString method() const;

    inline QByteArray toUtf8() const;

protected:
    QSharedPointer<Fw::JSON::Object> m_object;
};

///////////////////////////////////////////////////////////////////////////////

class Fw::JSON::RPC::Request : public Fw::JSON::RPC::Sentence
{
public:
    Request(int id, const QString& method);

    inline Fw::JSON::Object* params();
    inline const Fw::JSON::Object* const params() const;
};

////////////////////////////////////////////////////////////////////////////////

class Fw::JSON::RPC::Response : public Fw::JSON::RPC::Sentence
{
public:
    Response();

    inline const Fw::JSON::Object* const result() const;
    inline const Fw::JSON::Object* const error() const;

    inline int errorCode() const;
    inline QString errorMessage() const;
    inline const Fw::JSON::Object* const errorData() const;

    inline void setNetworkError(const QByteArray&);
    inline const QByteArray& networkError() const;

private:
    QByteArray m_error;
};

#endif // FIREWORKS_JSONRPC_HPP
