#ifndef FIREWORKS_ML_INL_H
#define FIREWORKS_ML_INL_H

#include "fwcore/fwml.h"

FwMLNode::Type FwMLNode::type() const
{
    return m_type;
}

bool FwMLNode::isNull() const
{
    return m_type == T_Null;
}

FwMLString* FwMLNode::toString()
{
    if(m_type == T_String)
    {
        return static_cast<FwMLString*>(const_cast<FwMLNode*>(this));
    }
    return 0;
}

FwMLUIntNumber* FwMLNode::toUIntNumber()
{
    if(m_type == T_UIntNumber)
    {
        return static_cast<FwMLUIntNumber*>(this);
    }
    return 0;
}

FwMLObject* FwMLNode::toObject()
{
    if(m_type == T_Object)
    {
        return static_cast<FwMLObject*>(this);
    }
    return 0;
}

FwMLArray* FwMLNode::toArray()
{
    if(m_type == T_Array)
    {
        return static_cast<FwMLArray*>(this);
    }
    return 0;
}

FwMLNode* FwMLNode::parent() const
{
    return m_parent;
}

////////////////////////////////////////////////////////////////////////////////

quint32 FwMLUIntNumber::value() const
{
    return m_value;
}

void FwMLUIntNumber::setValue(quint32 value)
{
    m_value = value;
}

////////////////////////////////////////////////////////////////////////////////

FwMLString* FwMLObject::addAttribute(const QByteArray& name, const QByteArray& value)
{
    FwMLString* string = new FwMLString(value);
    addAttribute(name, string);
    return string;
}

FwMLString* FwMLObject::addAttribute(const QByteArray& name, const QString& value)
{
    return addAttribute(name, value.toUtf8());
}

FwMLString* FwMLObject::addAttribute(const QByteArray &name, const QUrl& url)
{
    return addAttribute(name, url.toEncoded());
}

FwMLArray* FwMLObject::addAttribute(const QByteArray& name, const QVector<FwMLNode*> array)
{
    FwMLArray* jsonArray = new FwMLArray();
    jsonArray->data = array;
    addAttribute(name, jsonArray);
    return jsonArray;
}

FwMLNode* FwMLObject::attribute(const QByteArray& name) const
{
    return m_attributes.value(name, 0);
}

QHash<QByteArray, FwMLNode*> FwMLObject::attributes() const
{
    return m_attributes;
}

void FwMLObject::removeAttribute(const QByteArray& name)
{
    if(m_attributes.contains(name))
    {
        delete m_attributes.take(name);
    }
}


////////////////////////////////////////////////////////////////////////////////

int FwMLArray::size() const
{
    return data.size();
}

#endif // FIREWORKS_ML_INL_H
