#ifndef FIREWORKS_ML_H
#define FIREWORKS_ML_H

#include "fireworks.h"

#include <QtCore/qbytearray.h>
#include <QtCore/qhash.h>
#include <QtCore/qvector.h>
#include <QtCore/qurl.h>

#include "fwcore/fwcolor.h"

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
        T_FloatNumber,
        T_DoubleNumber,
        T_Object,
        T_Array,
    };

    FwMLNode();
    FwMLNode(const QByteArray& attrName, FwMLObject* parent);
    FwMLNode(FwMLArray* parent);
    virtual ~FwMLNode();

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

    inline FwMLNode* parent() const;

    void takeFromParent();

    virtual int toInt(bool* bOk) const = 0;
    virtual bool toBool(bool* bOk) const = 0;
    virtual FwColor toColor(bool* bOk) const = 0;

    virtual FwMLNode* clone() const = 0;

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

class FwMLString : public FwMLBase<FwMLNode::T_String>
{
    typedef FwMLBase<FwMLNode::T_String> BaseClass;
public:
    explicit FwMLString();
    explicit FwMLString(const QByteArray& value);
    FwMLString(const QByteArray &value, const QByteArray& attr, FwMLObject* parent);
    FwMLString(const QByteArray &value, FwMLArray* parent);

    QByteArray toUtf8() const;

    inline QByteArray value() const;
    inline void setValue(const QByteArray& value);

    int toInt(bool* bOk) const;
    bool toBool(bool* bOk) const;
    FwColor toColor(bool* bOk) const;
    quint32 toUInt(bool* bOk) const;

    FwMLNode* clone() const;

private:
    QByteArray m_value;
};

////////////////////////////////////////////////////////////////////////////////

class FwMLUIntNumber : public FwMLBase<FwMLNode::T_UIntNumber>
{
    typedef FwMLBase<FwMLNode::T_UIntNumber> BaseClass;
public:   
    FwMLUIntNumber();
    FwMLUIntNumber(quint32 value, const QByteArray& attrName, FwMLObject* parent);
    FwMLUIntNumber(quint32 value, FwMLArray* parent);

    inline quint32 value() const;
    inline void setValue(quint32 value);

    QByteArray toUtf8() const;

    int toInt(bool* bOk) const;
    bool toBool(bool* bOk) const;
    FwColor toColor(bool* bOk) const;

    FwMLNode* clone() const;

private:
    quint32 m_value;
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

    int toInt(bool* bOk) const;
    bool toBool(bool* bOk) const;
    FwColor toColor(bool* bOk) const;

    FwMLNode* clone() const;

private:
   int m_value;
};

////////////////////////////////////////////////////////////////////////////////

class FwMLDoubleNumber : public FwMLBase<FwMLNode::T_DoubleNumber>
{
    typedef FwMLBase<FwMLNode::T_DoubleNumber> BaseClass;
public:
    FwMLDoubleNumber();
    FwMLDoubleNumber(double value);
    FwMLDoubleNumber(double value, const QByteArray& attrName, FwMLObject* parent);
    FwMLDoubleNumber(double value, FwMLArray* parent);

    inline double value() const;
    inline void setValue(double value);

    QByteArray toUtf8() const;

    int toInt(bool* bOk) const;
    bool toBool(bool* bOk) const;
    FwColor toColor(bool* bOk) const;

    FwMLNode* clone() const;

private:
    double m_value;
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

    inline QByteArray className() const;

    void addAttribute(const QByteArray& name, FwMLNode* value, bool replace = true);

    inline FwMLNode* attribute(const QByteArray& name) const;
    inline QByteArray attributeName(FwMLNode* child) const;

    inline QHash<QByteArray, FwMLNode*> attributes() const;
    inline QList<FwMLNode*> toList() const;
    inline int attributesCount() const;

    inline void removeAttribute(const QByteArray& name);
    void removeAttributes();

    QByteArray toUtf8() const;

    bool parse(const QByteArray& utf8String, QString* error = 0);
    bool parse(QIODevice* ioDevice, QString* error = 0);

    int toInt(bool* bOk) const;
    bool toBool(bool* bOk) const;
    FwColor toColor(bool* bOk) const;

    FwMLNode* clone() const;

private:
    QHash<QByteArray, FwMLNode*> m_attributes;
};

////////////////////////////////////////////////////////////////////////////////

class FwMLArray : public FwMLBase<FwMLNode::T_Array>
{
    typedef FwMLBase<FwMLNode::T_Array> BaseClass;
public:

    friend class FwMLNode;

    FwMLArray();
    FwMLArray(const QByteArray& attrName, FwMLObject* parent);
    FwMLArray(FwMLArray* parent);
    ~FwMLArray();

    void addNode(FwMLNode* node);

    inline int size() const;

    inline int indexOf(FwMLNode* item) const;
    inline FwMLNode* item(int index) const;

    QByteArray toUtf8() const;

    int toInt(bool* bOk) const;
    bool toBool(bool* bOk) const;
    FwColor toColor(bool* bOk) const;

    FwMLNode* clone() const;

    inline QVector<FwMLNode*> toQVector() const;

private:
    QVector<FwMLNode*> m_data;
};

#include "fwml_inl.h"

#endif // FIREWORKS_ML_H
