#ifndef FIREWORKS_JSONRPC_INL_INL_HPP
#define FIREWORKS_JSONRPC_INL_INL_HPP

#include <QtCore/qbuffer.h>

#include "fw/jsonrpc/jsonrpc.hpp"

void Fw::JSON::RPC::Sentence::parse(QIODevice* ioDevice) throw(const Fw::Exception&)
{
    m_object->parse(ioDevice);
}

void Fw::JSON::RPC::Sentence::parse(const QString& json) throw(const Fw::Exception&)
{
    QByteArray utf8 = json.toUtf8();
    QBuffer buffer(&utf8);
    parse(&buffer);
}

void Fw::JSON::RPC::Sentence::valid() const throw(const Fw::Exception&)
{
    validation(m_object);
}

int Fw::JSON::RPC::Sentence::id() const
{
    return m_object->value<Fw::JSON::Number>("id");
}

void Fw::JSON::RPC::Sentence::setID(int id)
{
    m_object->setValue<Fw::JSON::Number>("id", id);
}

QByteArray Fw::JSON::RPC::Sentence::toUtf8() const
{
    return m_object->toUtf8();
}

Fw::JSON::RPC::Sentence::ObjectPointer Fw::JSON::RPC::Sentence::object() const
{
    return m_object;
}

////////////////////////////////////////////////////////////////////////////////

QString Fw::JSON::RPC::Request::method() const
{
    return object()->value<Fw::JSON::String>("method");
}

void Fw::JSON::RPC::Request::setMethod(const QString& method)
{
    object()->setValue<Fw::JSON::String>("method", method);
}

Fw::JSON::Node* Fw::JSON::RPC::Request::param() const
{
    return object()->attribute("param");
}

void Fw::JSON::RPC::Request::setParam(Fw::JSON::Node* param)
{
    object()->addAttribute("param", param);
}

////////////////////////////////////////////////////////////////////////////////

//void Fw::JSON::RPC::Response::parse(QIODevice* ioDevice) throw(Fw::Exception)
//{
//    object()->parse(ioDevice);
//}

const Fw::JSON::Object* const Fw::JSON::RPC::Response::result() const
{
    return object()->attribute("result")->cast<Fw::JSON::Object>();
}

const Fw::JSON::Object* const Fw::JSON::RPC::Response::error() const
{
    return object()->attribute("error")->cast<Fw::JSON::Object>();
}

inline void Fw::JSON::RPC::Response::setNetworkError(const QByteArray& error)
{
    m_error = error;
}

inline const QByteArray& Fw::JSON::RPC::Response::networkError() const
{
    return m_error;
}

int Fw::JSON::RPC::Response::errorCode() const
{
    return object()->value<Fw::JSON::Number>("code");
}

QString Fw::JSON::RPC::Response::errorMessage() const
{
    return object()->value<Fw::JSON::String>("message");
}

const Fw::JSON::Object* const Fw::JSON::RPC::Response::errorData() const
{
    return object()->attribute("data")->cast<Fw::JSON::Object>();
}

#endif // FIREWORKS_JSONRPC_INL_INL_HPP
