#ifndef FIREWORKS_JSONRPC_HPP
#define FIREWORKS_JSONRPC_HPP

#include <QtCore/qpointer.h>

#include <QtCore/QSharedPointer>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>

#include "fw/core/json.hpp"
#include "fw/core/exception.hpp"

#include "fw/jsonrpc/defs.hpp"

////////////////////////////////////////////////////////////////////////////////

class FW_JSONRPC_SHARED_EXPORT Fw::JSON::RPC : public QObject
{
    Q_OBJECT
    typedef QObject BaseClass;

public:

    class Sentence;
    class Request;
    class Response;

    RPC(QNetworkAccessManager* networkManager, QObject* parent = 0);

    void send(const QUrl& serverURL, const Request& request) throw (const Fw::Exception&);
    void send(const QUrl& serverURL, const QString& request) throw (const Fw::Exception&);

signals:
    void finished(const Fw::JSON::RPC::Response& response);
    void error(const QString& message);

private:
    QPointer<QNetworkAccessManager> m_networkManager;

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
    inline void valid() const throw(const Fw::Exception&);

    inline void parse(QIODevice* ioDevice) throw(const Fw::Exception&);
    inline void parse(const QString& json) throw(const Fw::Exception&);

    inline int id() const;
    inline void setID(int id);

    inline QByteArray toUtf8() const;

    inline ObjectPointer object() const;

protected:
    virtual void validation(const ObjectPointer& object) const throw(const Fw::Exception&) = 0;

    ObjectPointer m_object;
};

///////////////////////////////////////////////////////////////////////////////

class FW_JSONRPC_SHARED_EXPORT Fw::JSON::RPC::Request : public Fw::JSON::RPC::Sentence
{
    typedef Fw::JSON::RPC::Sentence BaseClass;

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
    typedef Fw::JSON::RPC::Sentence BaseClass;
public: 
    Response(int id = 0);

    inline Fw::JSON::Node* result() const;
    Fw::JSON::Object* resultObject() const throw (const Fw::Exception&);

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
