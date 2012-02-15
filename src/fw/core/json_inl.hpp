#ifndef FIREWORKS_CORE_JSON_INL_HPP
#define FIREWORKS_CORE_JSON_INL_HPP

#include "fw/core/json.hpp"

Fw::JSON::Node* Fw::JSON::Node::parent() const
{
    return m_parent;
}

///////////////////////////////////////////////////////////////////////////////

bool Fw::JSON::String::isEmpty() const
{
    return value().isEmpty();
}

///////////////////////////////////////////////////////////////////////////////

Fw::JSON::Node* Fw::JSON::Object::attribute(const QByteArray& name) const
{
    return m_attributes.value(name, 0);
}

QByteArray Fw::JSON::Object::attributeName(Fw::JSON::Node* child) const
{
    return m_attributes.key(child, "");
}

QHash<QByteArray, Fw::JSON::Node*> Fw::JSON::Object::attributes() const
{
    return m_attributes;
}

QList<Fw::JSON::Node*> Fw::JSON::Object::toList() const
{
    return m_attributes.values();
}

void Fw::JSON::Object::removeAttribute(const QByteArray& name)
{
    if(m_attributes.contains(name))
    {
        delete m_attributes.take(name);
    }
}

int Fw::JSON::Object::attributesCount() const
{
    return m_attributes.size();
}

Fw::JSON::String* Fw::JSON::Object::addString(const QByteArray& name, const QString& value)
{
    return static_cast<String*>(addAttribute(name, new String(value)));
}

Fw::JSON::Number* Fw::JSON::Object::addNumber(const QByteArray& name, double value)
{
    return static_cast<Number*>(addAttribute(name, new Number(value)));
}

Fw::JSON::Boolean* Fw::JSON::Object::addBoolean(const QByteArray& name, bool value)
{
    return static_cast<Boolean*>(addAttribute(name, new Boolean(value)));
}

Fw::JSON::Object* Fw::JSON::Object::addObject(const QByteArray& name)
{
    return static_cast<Object*>(addAttribute(name, new Object()));
}

Fw::JSON::Array* Fw::JSON::Object::addArray(const QByteArray& name)
{
    return static_cast<Array*>(addAttribute(name, new Array()));
}

///////////////////////////////////////////////////////////////////////////////

int Fw::JSON::Array::size() const
{
    return m_data.size();
}

int Fw::JSON::Array::indexOf(Fw::JSON::Node* item) const
{
    return m_data.indexOf(item);
}

Fw::JSON::Node* Fw::JSON::Array::item(int index) const
{
    if(index < m_data.size() && index >= 0)
    {
        return m_data.at(index);
    }
    return 0;
}

QVector<Fw::JSON::Node*> Fw::JSON::Array::toQVector() const
{
    return m_data;
}

Fw::JSON::String* Fw::JSON::Array::addString(const QString& value)
{
    return static_cast<String*>(addValue(new String(value)));
}

Fw::JSON::Number* Fw::JSON::Array::addNumber(double value)
{
    return static_cast<Number*>(addValue(new Number(value)));
}

Fw::JSON::Boolean* Fw::JSON::Array::addBoolean(bool value)
{
    return static_cast<Boolean*>(addValue(new Boolean(value)));
}

Fw::JSON::Object* Fw::JSON::Array::addObject()
{
    return static_cast<Object*>(addValue(new Object()));
}

Fw::JSON::Array* Fw::JSON::Array::addArray()
{
    return static_cast<Array*>(addValue(new Array()));
}

#endif // FIREWORKS_CORE_JSON_INL_HPP
