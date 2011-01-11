#ifndef FIREWORKS_ML_H
#define FIREWORKS_ML_H

#include "fireworks.h"

#include <QtCore/qbytearray.h>
#include <QtCore/qhash.h>
#include <QtCore/qvector.h>
#include <QtCore/qurl.h>

class FwMLNode;
class FwMLString;
class FwMLUIntNumber;
class FwMLObject;
class FwMLArray;

class FwMLNode
{
public:

    friend class FwMLObject;
    friend class FwMLArray;

    enum Type
    {
        T_Null,
        T_String,
        T_UIntNumber,
        T_IntNumber,
        T_Object,
        T_Array,
    };

    FwMLNode();
    FwMLNode(const QByteArray& attrName, FwMLObject* parent);
    FwMLNode(FwMLArray* parent);
    virtual ~FwMLNode();

    virtual int type() const = 0;
    inline bool isNull() const;

    template <class T> T* cast()
    {
        return type() == T::typeID ? static_cast<T*>(this) : 0;
    }

    virtual QByteArray toUtf8() const = 0;

    inline FwMLNode* parent() const;

    void takeFromParent();

private:
    FwMLNode* m_parent;
};

////////////////////////////////////////////////////////////////////////////////

template <int type_id> class FwMLBase : public FwMLNode
{
    typedef FwMLNode BaseClass;
public:

    FwMLBase() :
        BaseClass()
    {
    }

    FwMLBase(const QByteArray& attrName, FwMLObject* parent) :
        BaseClass(attrName, parent)
    {
    }

    FwMLBase(FwMLArray* parent) :
        BaseClass(parent)
    {
    }

    ~FwMLBase()
    {
    }

    static const int typeID = type_id;

    virtual int type() const
    {
        return typeID;
    }
};

////////////////////////////////////////////////////////////////////////////////

template <int type_id, typename value_type> class FwMLValue : public FwMLBase<type_id>
{
    typedef FwMLBase<type_id> BaseClass;

protected:
    value_type m_value;

public:

    FwMLValue(const value_type& defValue) :
        BaseClass(),
        m_value(defValue)
    {
    }

    FwMLValue(const value_type& defValue, const QByteArray& attrName, FwMLObject* parent) :
        BaseClass(attrName, parent),
        m_value(defValue)
    {
    }

    FwMLValue(const value_type& defValue, FwMLArray* parent) :
        BaseClass(parent),
        m_value(defValue)
    {
    }

    ~FwMLValue()
    {
    }

    inline value_type value() const
    {
        return m_value;
    }

    inline void setValue(value_type value)
    {
        m_value = value;
    }
};

////////////////////////////////////////////////////////////////////////////////

class FwMLString : public FwMLValue<FwMLNode::T_String, QByteArray>
{
    typedef FwMLValue<FwMLNode::T_String, QByteArray> BaseClass;
public:
    explicit FwMLString();
    explicit FwMLString(const QByteArray& value);
    FwMLString(const QByteArray &value, const QByteArray& attr, FwMLObject* parent);
    FwMLString(const QByteArray &value, FwMLArray* parent);

    QByteArray toUtf8() const;
};

////////////////////////////////////////////////////////////////////////////////

class FwMLUIntNumber : public FwMLValue<FwMLNode::T_UIntNumber, quint32>
{
    typedef FwMLValue<FwMLNode::T_UIntNumber, quint32> BaseClass;
public:   
    FwMLUIntNumber();
    FwMLUIntNumber(quint32 value, const QByteArray& attrName, FwMLObject* parent);
    FwMLUIntNumber(quint32 value, FwMLArray* parent);

    QByteArray toUtf8() const;
};

////////////////////////////////////////////////////////////////////////////////

class FwMLIntNumber : public FwMLBase<FwMLNode::T_IntNumber>
{
    typedef FwMLBase<FwMLNode::T_IntNumber> BaseClass;

public:
    FwMLIntNumber();
    FwMLIntNumber(int value, const QByteArray& attrName, FwMLObject* parent);
    FwMLIntNumber(int value, FwMLArray* parent);

    QByteArray toUtf8() const;

    inline int value() const;
    inline void setValue(int value);

private:
   int m_value;
};

////////////////////////////////////////////////////////////////////////////////

class FwMLObject : public FwMLBase<FwMLNode::T_Object>
{
    typedef FwMLBase<FwMLNode::T_Object> BaseClass;
public:

    friend class FwMLNode;

    FwMLObject();
    FwMLObject(const QByteArray& attrName, FwMLObject* parent);
    FwMLObject(FwMLArray* parent);
    ~FwMLObject();

    FwMLNode* addAttribute(const QByteArray& name, FwMLNode* value, bool replace = true);
    inline FwMLString* addAttribute(const QByteArray& name, const QByteArray& value);
    inline FwMLString* addAttribute(const QByteArray& name, const QString& value);
    inline FwMLString* addAttribute(const QByteArray &name, const QUrl& url);
    inline FwMLArray* addAttribute(const QByteArray& name, const QVector<FwMLNode*> array);

    inline FwMLNode* attribute(const QByteArray& name) const;
    inline QHash<QByteArray, FwMLNode*> attributes() const;

    inline void removeAttribute(const QByteArray& name);
    void removeAttributes();

    QByteArray toUtf8() const;

    bool parse(const QByteArray& utf8String, QString* error = 0);
    bool parse(QIODevice* ioDevice, QString* error = 0);

private:
    QHash<QByteArray, FwMLNode*> m_attributes;
};

////////////////////////////////////////////////////////////////////////////////

class FwMLArray : public FwMLBase<FwMLNode::T_Array>
{
    typedef FwMLBase<FwMLNode::T_Array> BaseClass;
public:
    FwMLArray();
    FwMLArray(const QByteArray& attrName, FwMLObject* parent);
    FwMLArray(FwMLArray* parent);
    ~FwMLArray();

    inline int size() const;

    QVector<FwMLNode*> data;

    QByteArray toUtf8() const;
};

#include "fwml_inl.h"

#endif // FIREWORKS_ML_H
