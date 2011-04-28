#ifndef FIREWORKS_ML_INL_H
#define FIREWORKS_ML_INL_H

#include "fwcore/fwml.h"

FwMLNode* FwMLNode::parent() const
{
    return m_parent;
}

////////////////////////////////////////////////////////////////////////////////

QByteArray FwMLString::value() const
{
    return m_value;
}

void FwMLString::setValue(const QByteArray& value)
{
    m_value = value;
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

int FwMLIntNumber::value() const
{
    return m_value;
}

void FwMLIntNumber::setValue(int value)
{
    m_value = value;
}

////////////////////////////////////////////////////////////////////////////////

double FwMLDoubleNumber::value() const
{
    return m_value;
}

void FwMLDoubleNumber::setValue(double value)
{
    m_value = value;
}

////////////////////////////////////////////////////////////////////////////////

FwMLNode* FwMLObject::attribute(const QByteArray& name) const
{
    return m_attributes.value(name, 0);
}

QByteArray FwMLObject::attributeName(FwMLNode* child) const
{
    return m_attributes.key(child, "");
}

QHash<QByteArray, FwMLNode*> FwMLObject::attributes() const
{
    return m_attributes;
}

QList<FwMLNode*> FwMLObject::toList() const
{
    return m_attributes.values();
}

void FwMLObject::removeAttribute(const QByteArray& name)
{
    if(m_attributes.contains(name))
    {
        delete m_attributes.take(name);
    }
}

int FwMLObject::attributesCount() const
{
    return m_attributes.size();
}

QByteArray FwMLObject::className() const
{
    FwMLString* classNode = attribute("class")->cast<FwMLString>();
    if(classNode)
    {
        return classNode->value();
    }
    return QByteArray();
}

////////////////////////////////////////////////////////////////////////////////

int FwMLArray::size() const
{
    return m_data.size();
}

int FwMLArray::indexOf(FwMLNode* item) const
{
    return m_data.indexOf(item);
}

FwMLNode* FwMLArray::item(int index) const
{
    if(index < m_data.size() && index >= 0)
    {
        return m_data.at(index);
    }
    return 0;
}

#endif // FIREWORKS_ML_INL_H
