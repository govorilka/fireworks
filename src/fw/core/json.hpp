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
        class Number;
        class Boolean;
        class Object;
        class Array;

        template <int type_id> class Base;
        template <typename T, int type_id> class BaseValue;


        enum NodeType
        {
            T_Null,
            T_String,
            T_Number,
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
    virtual uint toUint(bool* bOk) const = 0;
    virtual bool toBool(bool* bOk) const = 0;
    virtual double toNumber(bool* bOk) const = 0;
    virtual QString toString(bool* bOk) const = 0;

    virtual Fw::JSON::Node* clone() const = 0;

private:
    Fw::JSON::Node* m_parent;
};

////////////////////////////////////////////////////////////////////////////////

template <int type_id> class Fw::JSON::Base : public Fw::JSON::Node
{
    typedef Fw::JSON::Node BaseClass;

public:

    static const int typeID = type_id;

    virtual int type() const
    {
        return typeID;
    }
};

////////////////////////////////////////////////////////////////////////////////

template <typename T, int type_id> class Fw::JSON::BaseValue : public Fw::JSON::Base<type_id>
{
    typedef Fw::JSON::Node BaseClass;

public:

    typedef T BaseType;

    BaseValue(const BaseType& value) :
        m_value(value)
    {
    }

    inline const BaseType& value() const
    {
        return m_value;
    }

    inline void setValue(const BaseType& value)
    {
        m_value = value;
    }

private:
    BaseType m_value;
};


////////////////////////////////////////////////////////////////////////////////

class FW_CORE_SHARED_EXPORT Fw::JSON::String : public Fw::JSON::BaseValue<QString, Fw::JSON::T_String>
{
    typedef Fw::JSON::BaseValue<QString, Fw::JSON::T_String> BaseClass;

public:

    inline static BaseType defaultValue()
    {
        return QString();
    }

    explicit String(const QString& value = defaultValue());

    inline bool isEmpty() const;

    QByteArray toUtf8() const;

    virtual int toInt(bool* bOk) const;
    virtual uint toUint(bool* bOk) const;
    virtual bool toBool(bool* bOk) const;
    virtual double toNumber(bool* bOk) const;
    virtual QString toString(bool* bOk) const;

    Fw::JSON::Node* clone() const;
};

////////////////////////////////////////////////////////////////////////////////

class FW_CORE_SHARED_EXPORT Fw::JSON::Number : public Fw::JSON::BaseValue<double, Fw::JSON::T_Number>
{
    typedef Fw::JSON::BaseValue<double, Fw::JSON::T_Number> BaseClass;
public:

    inline static BaseType defaultValue()
    {
        return 0.;
    }

    Number(double value = defaultValue());

    QByteArray toUtf8() const;

    virtual int toInt(bool* bOk) const;
    virtual uint toUint(bool* bOk) const;
    virtual bool toBool(bool* bOk) const;
    virtual double toNumber(bool* bOk) const;
    virtual QString toString(bool* bOk) const;

    Fw::JSON::Node* clone() const;
};

/////////////////////////////////////////////////////////////////////////////////

class FW_CORE_SHARED_EXPORT Fw::JSON::Boolean: public Fw::JSON::BaseValue<bool, Fw::JSON::T_Bool>
{
  typedef Fw::JSON::BaseValue<bool, Fw::JSON::T_Bool> BaseClass;

public:

    inline static BaseType defaultValue()
    {
        return false;
    }

    Boolean(bool value = defaultValue());

    QByteArray toUtf8() const;

    virtual int toInt(bool* bOk) const;
    virtual uint toUint(bool* bOk) const;
    virtual bool toBool(bool* bOk) const;
    virtual double toNumber(bool* bOk) const;
    virtual QString toString(bool* bOk) const;

    Fw::JSON::Node* clone() const;
};

////////////////////////////////////////////////////////////////////////////////

class FW_CORE_SHARED_EXPORT Fw::JSON::Object : public Fw::JSON::Base<Fw::JSON::T_Object>
{
    typedef Fw::JSON::Base<Fw::JSON::T_Object> BaseClass;
public:

    friend class Fw::JSON::Node;

    Object();
    ~Object();

    Node* addAttribute(const QByteArray& name, Fw::JSON::Node* value, bool replace = true);
    inline void removeAttribute(const QByteArray& name);

    inline String* addString(const QByteArray& name, const QString& value);
    inline Number* addNumber(const QByteArray& name, double value);
    inline Boolean* addBoolean(const QByteArray& name, bool value);
    inline Object* addObject(const QByteArray& name);
    inline Array* addArray(const QByteArray& name);

    void clear();

    inline Fw::JSON::Node* attribute(const QByteArray& name) const;
    inline QByteArray attributeName(Fw::JSON::Node* child) const;  

    template<class T> bool hasValue(const QByteArray& name, typename T::BaseType* value = 0);
    template<class T> typename T::BaseType value(const QByteArray& name, const typename T::BaseType& defaultValue = T::defaultValue());

    inline QHash<QByteArray, Fw::JSON::Node*> attributes() const;
    inline QList<Fw::JSON::Node*> toList() const;
    inline int attributesCount() const;

    QByteArray toUtf8() const;

    void parse(const QByteArray& utf8String) throw(Fw::Exception);
    void parse(QIODevice* ioDevice) throw(Fw::Exception);
    void parseFile(const QString& fileName) throw(Fw::Exception);

    virtual int toInt(bool* bOk) const;
    virtual uint toUint(bool* bOk) const;
    virtual bool toBool(bool* bOk) const;
    virtual double toNumber(bool* bOk) const;
    virtual QString toString(bool* bOk) const;

    Fw::JSON::Node* clone() const;

private:
    QHash<QByteArray, Fw::JSON::Node*> m_attributes;
};

////////////////////////////////////////////////////////////////////////////////

class FW_CORE_SHARED_EXPORT Fw::JSON::Array : public Fw::JSON::Base<Fw::JSON::T_Array>
{
    typedef Fw::JSON::Base<Fw::JSON::T_Array> BaseClass;
public:

    friend class Fw::JSON::Node;

    Array();
    ~Array();

    void clear();

    Node* addValue(Fw::JSON::Node* node);

    inline String* addString(const QString& value);
    inline Number* addNumber(double value);
    inline Boolean* addBoolean(bool value);
    inline Object* addObject();
    inline Array* addArray();

    inline int size() const;

    inline int indexOf(Fw::JSON::Node* item) const;
    inline Fw::JSON::Node* item(int index) const;

    QByteArray toUtf8() const;

    virtual int toInt(bool* bOk) const;
    virtual uint toUint(bool* bOk) const;
    virtual bool toBool(bool* bOk) const;
    virtual double toNumber(bool* bOk) const;
    virtual QString toString(bool* bOk) const;

    Fw::JSON::Node* clone() const;

    inline QVector<Fw::JSON::Node*> toQVector() const;

private:
    QVector<Fw::JSON::Node*> m_data;
};

#include "fw/core/json_inl.hpp"

#endif // FIREWORKS_CORE_JSON_HPP
