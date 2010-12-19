#include "fwml.h"

FwMLNode::FwMLNode(Type type) :
    m_type(type)
{
}

FwMLNode::~FwMLNode()
{
}

////////////////////////////////////////////////////////////////////////////////

FwMLString::FwMLString() :
    BaseClass(FwMLNode::T_String)
{
}

FwMLString::FwMLString(const QByteArray& str) :
    BaseClass(FwMLNode::T_String),
    value(str)
{
}

QByteArray FwMLString::toUtf8() const
{
    return "\"" + value + "\"";
}

////////////////////////////////////////////////////////////////////////////////

FwMLNumber::FwMLNumber() :
    BaseClass(FwMLNode::T_Number),
    intValue(0),
    uintValue(0),
    realValue(0)
{
}

QByteArray FwMLNumber::toUtf8() const
{
    if(uintValue)
    {
        return QByteArray::number(uintValue);
    }
    else if(intValue)
    {
        return QByteArray::number(intValue);
    }
    else if(realValue)
    {
        return QByteArray::number(realValue);
    }
    return "0";
}

////////////////////////////////////////////////////////////////////////////////

FwMLObject::FwMLObject() :
    BaseClass(FwMLNode::T_Object)
{
}

FwMLObject::~FwMLObject()
{
    foreach(FwMLNode* node, m_attributes.values())
    {
        delete node;
    }
    m_attributes.clear();
}

/*!
Функция добавляет атрибут к объекту.
\param name Имя добавляемого аттрибута
\param value Значение добавляемого аттрибута
\param replace Определяет как вести себя, если атрибут с таким
именем уже имеется у объекта. Если значение истина, то при добавление
атрибут с таким же именем будет заменён на переданный в параметре value.
Если значение false, то будет создан массив создан массив включающий в себя
старое и новое значение. По умолчанию значение атрибута true.
*/
FwMLNode* FwMLObject::addAttribute(const QByteArray& name, FwMLNode* value, bool replace)
{
    if(m_attributes.contains(name))
    {
        if(replace)
        {
            delete m_attributes.take(name);
        }
        else
        {
            FwMLArray* addArray = m_attributes.value(name)->toArray();
            if(!addArray)
            {
                addArray = new FwMLArray();
                addArray->data.append(m_attributes.take(name));
                m_attributes.insert(name, addArray);
            }
            addArray->data.append(value);
            return value;
        }
    }

    m_attributes.insert(name, value);
    return value;
}

QByteArray FwMLObject::toUtf8() const
{
    QByteArray attributes;
    for(QHash<QByteArray, FwMLNode*>::const_iterator iter = m_attributes.begin(); iter != m_attributes.end(); ++iter)
    {
        if(!attributes.isEmpty())
        {
           attributes += ",";
        }
        FwMLNode* node = iter.value();
        QByteArray value = node->toUtf8();
        if(!value.isEmpty())
        {
            attributes += ("\"" + iter.key() + "\"");
            attributes += ":";
            attributes += value;
        }
    }

    if(attributes.isEmpty())
    {
        return "";
    }

    return "{" + attributes + "}";
}

////////////////////////////////////////////////////////////////////////////////

FwMLArray::FwMLArray() :
    BaseClass(FwMLNode::T_Array)
{
}

FwMLArray::~FwMLArray()
{
    foreach(FwMLNode* node, data)
    {
        delete node;
    }
    data.clear();
}

QByteArray FwMLArray::toUtf8() const
{
    QByteArray items;
    foreach(FwMLNode* node, data)
    {
        if(!items.isEmpty())
        {
            items += ",";
        }
        items += node->toUtf8();
    }
    if(items.isEmpty())
    {
        return QByteArray();
    }
    return "[" + items + "]";
}
