#ifndef FIREWORKS_CORE_JSON_HPP
#define FIREWORKS_CORE_JSON_HPP

#include <QtCore/qhash.h>
#include <QtCore/qvector.h>
#include <QtCore/qurl.h>

#include "fw/core/defs.hpp"
#include "fw/core/exception.hpp"

namespace Fw
{
    namespace JSON
    {
        class Node;
        class String;
        class UIntNumber;
        class IntNumber;
        class DoubleNumber;
        class Boolean;
        class Object;
        class Array;

        template <int type_id> class Base;

        enum NodeType
        {
            T_Null,
            T_String,
            T_UIntNumber,
            T_IntNumber,
            T_FloatNumber,
            T_DoubleNumber,
            T_Bool,
            T_Object,
            T_Array
        };

        const char constantTrue[] = "true";
        const char constantFalse[] = "false";

        FW_CORE_SHARED_EXPORT bool nameToBool(const QByteArray&, bool* bOk);
        FW_CORE_SHARED_EXPORT QByteArray boolToName(bool value);
    }
}

////////////////////////////////////////////////////////////////////////////////

class FW_CORE_SHARED_EXPORT Fw::JSON::Node
{
public:

    friend class Fw::JSON::Object;
    friend class Fw::JSON::Array;

    Node();
    Node(const QByteArray& attrName, Fw::JSON::Object* parent);
    Node(Fw::JSON::Array* parent);
    virtual ~Node();

    virtual int type() const = 0;
    inline bool isNull() const;

    QByteArray name() const;

    template <class T> T* cast()
    {
        if(this)
        {
            return type() == T::typeID ? static_cast<T*>(this) : 0;
        }
        return 0;
    }

    virtual QByteArray toUtf8() const = 0;

    inline Fw::JSON::Node* parent() const;

    void takeFromParent();

    virtual int toInt(bool* bOk) const = 0;
    virtual bool toBool(bool* bOk) const = 0;

    virtual Fw::JSON::Node* clone() const = 0;

private:
    Fw::JSON::Node* m_parent;
};

Fw::JSON::Node* Fw::JSON::Node::parent() const
{
    return m_parent;
}

////////////////////////////////////////////////////////////////////////////////

template <int type_id> class Fw::JSON::Base : public Fw::JSON::Node
{
    typedef Fw::JSON::Node BaseClass;
public:

    Base() :
        BaseClass()
    {
    }

    Base(const QByteArray& attrName, Fw::JSON::Object* parent) :
        BaseClass(attrName, parent)
    {
    }

    Base(Fw::JSON::Array* parent) :
        BaseClass(parent)
    {
    }

    ~Base()
    {
    }

    static const int typeID = type_id;

    virtual int type() const
    {
        return typeID;
    }
};

////////////////////////////////////////////////////////////////////////////////

class FW_CORE_SHARED_EXPORT Fw::JSON::String : public Fw::JSON::Base<Fw::JSON::T_String>
{
    typedef Fw::JSON::Base<Fw::JSON::T_String> BaseClass;

public:
    explicit String();
    explicit String(const QByteArray& value);
    String(const QByteArray &value, const QByteArray& attr, Fw::JSON::Object* parent);
    String(const QByteArray &value, Fw::JSON::Array* parent);

    inline QByteArray value() const;
    inline void setValue(const QByteArray& value);

    inline bool isEmpty() const;

    int toInt(bool* bOk) const;
    bool toBool(bool* bOk) const;
    quint32 toUInt(bool* bOk) const;
    QByteArray toUtf8() const;
    QString  toString() const;

    Fw::JSON::Node* clone() const;

private:
    QByteArray m_value;
};

QByteArray Fw::JSON::String::value() const
{
    return m_value;
}

void Fw::JSON::String::setValue(const QByteArray& value)
{
    m_value = value;
}

bool Fw::JSON::String::isEmpty() const
{
    return m_value.isEmpty();
}

////////////////////////////////////////////////////////////////////////////////

class FW_CORE_SHARED_EXPORT Fw::JSON::UIntNumber : public Fw::JSON::Base<Fw::JSON::T_UIntNumber>
{
    typedef Fw::JSON::Base<Fw::JSON::T_UIntNumber> BaseClass;
public:
    explicit UIntNumber();
    explicit UIntNumber(quint32 value);
    explicit UIntNumber(quint32 value, const QByteArray& attrName, Fw::JSON::Object* parent);
    explicit UIntNumber(quint32 value, Fw::JSON::Array* parent);

    inline quint32 value() const;
    inline void setValue(quint32 value);

    QByteArray toUtf8() const;

    int toInt(bool* bOk) const;
    bool toBool(bool* bOk) const;

    Fw::JSON::Node* clone() const;

private:
    quint32 m_value;
};

quint32 Fw::JSON::UIntNumber::value() const
{
    return m_value;
}

void Fw::JSON::UIntNumber::setValue(quint32 value)
{
    m_value = value;
}

////////////////////////////////////////////////////////////////////////////////

class FW_CORE_SHARED_EXPORT Fw::JSON::IntNumber : public Fw::JSON::Base<Fw::JSON::T_IntNumber>
{
    typedef Fw::JSON::Base<Fw::JSON::T_IntNumber> BaseClass;

public:
    IntNumber();
    IntNumber(int value, const QByteArray& attrName, Fw::JSON::Object* parent);
    IntNumber(int value, Fw::JSON::Array* parent);

    QByteArray toUtf8() const;

    inline int value() const;
    inline void setValue(int value);

    int toInt(bool* bOk) const;
    bool toBool(bool* bOk) const;

    Fw::JSON::Node* clone() const;

private:
   int m_value;
};

int Fw::JSON::IntNumber::value() const
{
    return m_value;
}

void Fw::JSON::IntNumber::setValue(int value)
{
    m_value = value;
}

////////////////////////////////////////////////////////////////////////////////

class FW_CORE_SHARED_EXPORT Fw::JSON::DoubleNumber : public Fw::JSON::Base<Fw::JSON::T_DoubleNumber>
{
    typedef Fw::JSON::Base<Fw::JSON::T_DoubleNumber> BaseClass;
public:
    DoubleNumber();
    DoubleNumber(double value);
    DoubleNumber(double value, const QByteArray& attrName, Fw::JSON::Object* parent);
    DoubleNumber(double value, Fw::JSON::Array* parent);

    inline double value() const;
    inline void setValue(double value);

    QByteArray toUtf8() const;

    int toInt(bool* bOk) const;
    bool toBool(bool* bOk) const;

    Fw::JSON::Node* clone() const;

private:
    double m_value;
};


double Fw::JSON::DoubleNumber::value() const
{
    return m_value;
}

void Fw::JSON::DoubleNumber::setValue(double value)
{
    m_value = value;
}

////////////////////////////////////////////////////////////////////////////////

class FW_CORE_SHARED_EXPORT Fw::JSON::Object : public Fw::JSON::Base<Fw::JSON::T_Object>
{
    typedef Fw::JSON::Base<Fw::JSON::T_Object> BaseClass;
public:

    friend class Fw::JSON::Node;

    Object();
    Object(const QByteArray& attrName, Fw::JSON::Object* parent);
    Object(Fw::JSON::Array* parent);
    ~Object();

    void clear();

    inline QByteArray className() const;
    inline QByteArray baseName() const;

    inline QByteArray stringValue(const QByteArray& attrName) const;

    void addAttribute(const QByteArray& name, Fw::JSON::Node* value, bool replace = true);

    inline Fw::JSON::Node* attribute(const QByteArray& name) const;
    inline QByteArray attributeName(Fw::JSON::Node* child) const;

    inline QHash<QByteArray, Fw::JSON::Node*> attributes() const;
    inline QList<Fw::JSON::Node*> toList() const;
    inline int attributesCount() const;

    inline void removeAttribute(const QByteArray& name);

    QByteArray toUtf8() const;

    void parse(const QByteArray& utf8String) throw(Fw::Exception);
    void parse(QIODevice* ioDevice) throw(Fw::Exception);
    void parseFile(const QString& fileName) throw(Fw::Exception);

    int toInt(bool* bOk) const;
    bool toBool(bool* bOk) const;

    Fw::JSON::Node* clone() const;

private:
    QHash<QByteArray, Fw::JSON::Node*> m_attributes;
};

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

QByteArray Fw::JSON::Object::className() const
{
    return stringValue("class");
}

QByteArray Fw::JSON::Object::baseName() const
{
    return stringValue("base").simplified().toLower();
}

QByteArray Fw::JSON::Object::stringValue(const QByteArray& attrName) const
{
    Fw::JSON::String* classNode = attribute(attrName)->cast<Fw::JSON::String>();
    if(classNode)
    {
        return classNode->value();
    }
    return QByteArray();
}

////////////////////////////////////////////////////////////////////////////////

class FW_CORE_SHARED_EXPORT Fw::JSON::Array : public Fw::JSON::Base<Fw::JSON::T_Array>
{
    typedef Fw::JSON::Base<Fw::JSON::T_Array> BaseClass;
public:

    friend class Fw::JSON::Node;

    Array();
    Array(const QByteArray& attrName, Fw::JSON::Object* parent);
    Array(Fw::JSON::Array* parent);
    ~Array();

    void addNode(Fw::JSON::Node* node);

    inline int size() const;

    inline int indexOf(Fw::JSON::Node* item) const;
    inline Fw::JSON::Node* item(int index) const;

    QByteArray toUtf8() const;

    int toInt(bool* bOk) const;
    bool toBool(bool* bOk) const;

    Fw::JSON::Node* clone() const;

    inline QVector<Fw::JSON::Node*> toQVector() const;

private:
    QVector<Fw::JSON::Node*> m_data;
};

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

/////////////////////////////////////////////////////////////////////////////////

class FW_CORE_SHARED_EXPORT Fw::JSON::Boolean: public Fw::JSON::Base<Fw::JSON::T_Bool>
{
  typedef Fw::JSON::Base<Fw::JSON::T_Bool> BaseClass;

public:
    friend class Fw::JSON::Node;

    Boolean();
    Boolean(bool value);
    Boolean(bool value, const QByteArray& attrName, Fw::JSON::Object* parent);
    Boolean(bool value, Fw::JSON::Array* parent);

    inline bool value() const;
    inline void setValue(bool value);

    QByteArray toUtf8() const;

    int toInt(bool* bOk) const;
    bool toBool(bool *bOk) const;
    quint32 toUInt(bool* bOk) const;

    Fw::JSON::Node* clone() const;

private:
    bool m_value;
};

bool Fw::JSON::Boolean::value() const
{
    return m_value;
}

void Fw::JSON::Boolean::setValue(bool value)
{
    m_value = value;
}

#endif // FIREWORKS_CORE_JSON_HPP
