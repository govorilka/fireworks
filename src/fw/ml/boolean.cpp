#include "boolean.hpp"

Fw::ML::Boolean::Boolean() :
    BaseClass(),
    m_value(false)
{
}

Fw::ML::Boolean::Boolean(bool value) :
    BaseClass(),
    m_value(value)
{
}

Fw::ML::Boolean::Boolean(bool value, const QByteArray &attrName, Object *parent) :
    BaseClass(attrName, parent),
    m_value(value)
{
}

Fw::ML::Boolean::Boolean(bool value, Array* parent) :
    BaseClass(parent),
    m_value(value)
{
}

QByteArray Fw::ML::Boolean::toUtf8() const
{
    return m_value ? Fw::constantTrue : Fw::constantFalse;
}

int Fw::ML::Boolean::toInt(bool *bOk) const
{
    (*bOk) = true;
    return m_value;
}

quint32 Fw::ML::Boolean::toUInt(bool* bOk) const
{
    (*bOk) = true;
    return m_value;
}

Fw::ML::Node* Fw::ML::Boolean::clone() const
{
    return new Boolean(value());
}

bool Fw::ML::Boolean::toBool(bool *bOk) const
{
    (*bOk) = true;
    return m_value;
}

FwColor Fw::ML::Boolean::toColor(bool *bOk) const
{
    (*bOk) = false;
    return FwColor();
}
