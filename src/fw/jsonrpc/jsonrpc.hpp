#ifndef FIREWORKS_JSONRPC_HPP
#define FIREWORKS_JSONRPC_HPP

#include <QtCore/QSharedPointer>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>

#include "fw/core/json.hpp"
#include "fw/core/exception.hpp"

#include "fw/jsonrpc/defs.hpp"

namespace Fw
{
    namespace JSON
    {
        class RPC;
    }
}

////////////////////////////////////////////////////////////////////////////////

class FW_JSONRPC_SHARED_EXPORT Fw::JSON::RPC : public QObject
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

class FW_JSONRPC_SHARED_EXPORT Fw::JSON::RPC::Sentence
{
public:

    typedef QSharedPointer<Fw::JSON::Object> ObjectPointer;

    Sentence(int id = 0);

    bool isValid(QByteArray* errorMessage = 0) const;

    void parse(QIODevice* ioDevice) throw(const Fw::Exception&);

    inline int id() const;
    inline void setID(int id);

    inline QByteArray toUtf8() const;

protected:
    virtual void validation(const ObjectPointer& object) const throw(const Fw::Exception&) = 0;

    ObjectPointer m_object;
};

///////////////////////////////////////////////////////////////////////////////

class FW_JSONRPC_SHARED_EXPORT Fw::JSON::RPC::Request : public Fw::JSON::RPC::Sentence
{
    typedef Sentence BaseClass;

public:
    Request(int id = 0, const QString& method = QString(), Node *params = 0);

    inline QString method() const;
    inline void setMethod(const QString& method);

    inline Fw::JSON::Node* param() const;
    inline void setParam(Fw::JSON::Node* param);

protected:
    void validation(const ObjectPointer& object) const throw(const Fw::Exception&);
};

////////////////////////////////////////////////////////////////////////////////

class FW_JSONRPC_SHARED_EXPORT Fw::JSON::RPC::Response : public Fw::JSON::RPC::Sentence
{
    typedef Sentence BaseClass;
public: 
    Response(int id = 0);

    inline const Fw::JSON::Object* const result() const;
    inline const Fw::JSON::Object* const error() const;

    inline int errorCode() const;
    inline QString errorMessage() const;
    inline const Fw::JSON::Object* const errorData() const;

    inline void setNetworkError(const QByteArray&);
    inline const QByteArray& networkError() const;

protected:
    void validation(const ObjectPointer& object) const throw(const Fw::Exception&);

private:
    QByteArray m_error;
};

#include "jsonrpc_inl.hpp"

#endif // FIREWORKS_JSONRPC_HPP
