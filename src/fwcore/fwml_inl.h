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

FwMLNode* FwMLNode::parent() const
{
    return m_parent;
}

////////////////////////////////////////////////////////////////////////////////

FwMLNumber::NumberType FwMLNumber::numberType() const
{
    return m_numberType;
}

qint32 FwMLNumber::toInt() const
{
    switch(m_numberType)
    {
    case NT_Int:
        return m_intValue;

    case NT_UInt:
        return m_uintValue < INT_MAX ? m_intValue : 0;

    case NT_Real:
        return m_realValue < INT_MAX ? qRound(m_realValue) : 0;
    }
    return 0;
}

void FwMLNumber::setIntValue(qint32 intValue)
{
    m_numberType = NT_Int;
    m_intValue = intValue;
}

quint32 FwMLNumber::toUInt() const
{
    switch(m_numberType)
    {
    case NT_Int:
        return m_intValue > 0 ? m_uintValue : 0;

    case NT_UInt:
        return m_uintValue;

    case NT_Real:
        return m_realValue < UINT_MAX ? qRound(m_realValue) : 0;
    }
    return 0;
}

void FwMLNumber::setUIntValue(quint32 uintValue)
{
    m_numberType = NT_UInt;
    m_uintValue = uintValue;
}

qreal FwMLNumber::toReal() const
{
    switch(m_numberType)
    {
    case NT_Int:
        return m_intValue;

    case NT_UInt:
        return m_uintValue;

    case NT_Real:
        return m_realValue;
    }
    return 0;
}

void FwMLNumber::setRealValue(qreal realValue)
{
    m_numberType = NT_Real;
    m_realValue = realValue;
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

#endif // FIREWORKS_ML_INL_H
