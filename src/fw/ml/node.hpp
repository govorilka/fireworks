#ifndef FIREWORKS_ML_NODE_H
#define FIREWORKS_ML_NODE_H

#include "fwcore/fwcolor.h"

#include "fw/ml/defs.hpp"

class FIREWORKSSHARED_EXPORT Fw::ML::Node
{
    Node* m_parent;

public:
    friend class Fw::ML::Object;
    friend class Fw::ML::Array;

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

    inline Fw::ML::Node* parent() const;

    void takeFromParent();

    virtual int toInt(bool* bOk) const = 0;
    virtual bool toBool(bool* bOk) const = 0;
    virtual FwColor toColor(bool* bOk) const = 0;

    virtual Node* clone() const = 0;
};

Fw::ML::Node* Fw::ML::Node::parent() const
{
    return m_parent;
}

////////////////////////////////////////////////////////////////////////////////

template <int type_id> class Fw::ML::BaseTempl : public Fw::ML::Node
{
    typedef Fw::ML::Node BaseClass;
public:

    BaseTempl() :
        BaseClass()
    {
    }

    ~BaseTempl()
    {
    }

    static const int typeID = type_id;

    virtual int type() const
    {
        return typeID;
    }
};

#endif //FIREWORKS_ML_NODE_H
