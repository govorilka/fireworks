#ifndef FIREWORKS_ML_OBJECT_H
#define FIREWORKS_ML_OBJECT_H

#include <QtCore/qhash.h>
#include <QtCore/qvector.h>

#include "fw/ml/node.hpp"

namespace Fw
{
    namespace ML
    {
        typedef QHash<QByteArray, Node*> NodeHash;
    }
}

class FIREWORKSSHARED_EXPORT Fw::ML::Object : public Fw::ML::BaseTempl<Fw::ML::T_Object>
{
    typedef Fw::ML::BaseTempl<Fw::ML::T_Object> BaseClass;

    NodeHash m_attributes;

    Object* m_classObject;
    QVector<Object*> m_inheritedObjects;

public:

    friend class Node;

    Object();
    Object(const QByteArray& attrName, Object* parent);
    Object(Array* parent);
    ~Object();

    inline QByteArray className() const;
    inline QByteArray baseName() const;

    QByteArray stringValue(const QByteArray& attrName) const;

    inline Object* classObject() const;
    void setClassObject(Object* object);

    void addAttribute(const QByteArray& name, Node* value, bool replace = true);

    inline Node* attribute(const QByteArray& name) const;
    inline QByteArray attributeName(Node* child) const;

    inline int attributesCount() const;

    inline void removeAttribute(const QByteArray& name);
    void removeAttributes();

    virtual QByteArray toUtf8() const;
    inline NodeHash toHash() const;
    inline QList<Node*> toList() const;

    int toInt(bool* bOk) const;
    bool toBool(bool* bOk) const;
    FwColor toColor(bool* bOk) const;

    Node* clone() const;
};

Fw::ML::Node* Fw::ML::Object::attribute(const QByteArray& name) const
{
    Node* attr = m_attributes.value(name, 0);
    if(!attr && m_classObject)
    {
        return m_classObject->attribute(name);
    }
    return attr;
}

QByteArray Fw::ML::Object::attributeName(Node* child) const
{
    return m_attributes.key(child, "");
}

Fw::ML::NodeHash Fw::ML::Object::toHash() const
{
    return m_attributes;
}

QList<Fw::ML::Node*> Fw::ML::Object::toList() const
{
    return m_attributes.values();
}

void Fw::ML::Object::removeAttribute(const QByteArray& name)
{
    if(m_attributes.contains(name))
    {
        delete m_attributes.take(name);
    }
}

int Fw::ML::Object::attributesCount() const
{
    return m_attributes.size();
}

QByteArray Fw::ML::Object::className() const
{
    return stringValue("class");
}

QByteArray Fw::ML::Object::baseName() const
{
    return stringValue("base").simplified().toLower();
}

Fw::ML::Object* Fw::ML::Object::classObject() const
{
    return m_classObject;
}

#endif // FIREWORKS_ML_OBJECT_H
