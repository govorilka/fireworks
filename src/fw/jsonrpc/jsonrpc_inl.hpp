#ifndef FIREWORKS_JSONRPC_INL_INL_HPP
#define FIREWORKS_JSONRPC_INL_INL_HPP

#include "fw/jsonrpc/jsonrpc.hpp"

int Fw::JSON::RPC::Sentence::id() const
{
    return m_object->value<Fw::JSON::Number>("id");
}

void Fw::JSON::RPC::Sentence::setID(int id)
{
    m_object->setValue<Fw::JSON::Number>("id", id);
}

QString Fw::JSON::RPC::Sentence::method() const
{
    return m_object->value<Fw::JSON::String>("method");
}

void Fw::JSON::RPC::Sentence::setMethod(const QString& method) const
{
    m_object->setValue<Fw::JSON::String>("method", method);
}

QByteArray Fw::JSON::RPC::Sentence::toUtf8() const
{
    return m_object->toUtf8();
}

////////////////////////////////////////////////////////////////////////////////

Fw::JSON::Object* Fw::JSON::RPC::Request::params()
{
    return const_cast<Fw::JSON::Object*>(params());
}

const Fw::JSON::Object* const Fw::JSON::RPC::Request::params() const
{
    return m_object->attribute("params")->cast<Fw::JSON::Object>();
}

////////////////////////////////////////////////////////////////////////////////

//void Fw::JSON::RPC::Response::parse(QIODevice* ioDevice) throw(Fw::Exception)
//{
//    m_object->parse(ioDevice);
//}

const Fw::JSON::Object* const Fw::JSON::RPC::Response::result() const
{
    return m_object->attribute("result")->cast<Fw::JSON::Object>();
}

const Fw::JSON::Object* const Fw::JSON::RPC::Response::error() const
{
    return m_object->attribute("error")->cast<Fw::JSON::Object>();
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
    return m_object->value<Fw::JSON::Number>("code");
}

QString Fw::JSON::RPC::Response::errorMessage() const
{
    return m_object->value<Fw::JSON::String>("message");
}

const Fw::JSON::Object* const Fw::JSON::RPC::Response::errorData() const
{
    return m_object->attribute("data")->cast<Fw::JSON::Object>();
}


#endif // FIREWORKS_JSONRPC_INL_INL_HPP
