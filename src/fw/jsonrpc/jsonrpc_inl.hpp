#ifndef FIREWORKS_JSONRPC_INL_INL_HPP
#define FIREWORKS_JSONRPC_INL_INL_HPP

#include "fw/jsonrpc/jsonrpc.hpp"

bool Fw::JSON::RPC::BaseRPC::isValid() const
{
    return id() && !method().isEmpty();
}

int Fw::JSON::RPC::BaseRPC::id() const
{
    return m_object->numberAttribute("id");
}

QString Fw::JSON::RPC::BaseRPC::method() const
{
    return m_object->stringAttribute("method");
}

QByteArray Fw::JSON::RPC::BaseRPC::toUtf8() const
{
    return m_object->toUtf8();
}

////////////////////////////////////////////////////////////////////////////////

Fw::JSON::Object* Fw::JSON::RPC::Request::params()
{
    return m_object->objectAttribute("params");
}

const Fw::JSON::Object* const Fw::JSON::RPC::Request::params() const
{
    return m_object->objectAttribute("params");
}

////////////////////////////////////////////////////////////////////////////////

void Fw::JSON::RPC::Response::parse(QIODevice* ioDevice) throw(Fw::Exception)
{
    m_object->parse(ioDevice);
}

const Fw::JSON::Object* const Fw::JSON::RPC::Response::result() const
{
    return m_object->objectAttribute("result");
}

const Fw::JSON::Object* const Fw::JSON::RPC::Response::error() const
{
    return m_object->objectAttribute("error");
}

inline void Fw::JSON::RPC::Response::setNetworkError(const QByteArray& error)
{
    m_error = error;
}

inline const QByteArray& Fw::JSON::RPC::Response::networkError() const
{
    return m_error;
}

#endif // FIREWORKS_JSONRPC_INL_INL_HPP
