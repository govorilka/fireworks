#ifndef FIREWORKS_ML_INL_H
#define FIREWORKS_ML_INL_H

#include "core/fwml.h"

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

FwMLNumber* FwMLNode::toNumber()
{
    if(m_type == T_Number)
    {
        return static_cast<FwMLNumber*>(this);
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

FwMLNumber* FwMLObject::addAttribute(const QByteArray& name, int value)
{
    FwMLNumber* number = new FwMLNumber();
    number->intValue = value;
    addAttribute(name, number);
    return number;
}

FwMLNumber* FwMLObject::addAttribute(const QByteArray &name, quint32 value)
{
    FwMLNumber* number = new FwMLNumber();
    number->uintValue = value;
    addAttribute(name, number);
    return number;
}

FwMLNumber* FwMLObject::addAttribute(const QByteArray& name, qreal value)
{
    FwMLNumber* number = new FwMLNumber();
    number->intValue = value;
    addAttribute(name, number);
    return number;
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

void FwMLObject::removeAttribute(const QByteArray& name)
{
    if(m_attributes.contains(name))
    {
        delete m_attributes.take(name);
    }
}

#endif // FIREWORKS_ML_INL_H
