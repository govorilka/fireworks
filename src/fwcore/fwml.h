#ifndef FIREWORKS_ML_H
#define FIREWORKS_ML_H

#include "fireworks.h"

#include <QtCore/qbytearray.h>
#include <QtCore/qhash.h>
#include <QtCore/qvector.h>
#include <QtCore/qurl.h>

class FwMLNode;
class FwMLString;
class FwMLNumber;
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
        T_Number,
        T_Object,
        T_Array,
    };

    FwMLNode(Type type);
    FwMLNode(Type type, const QByteArray& attrName, FwMLObject* parent);
    virtual ~FwMLNode();

    inline Type type() const;
    inline bool isNull() const;

    inline FwMLString* toString();
    inline FwMLNumber* toNumber();
    inline FwMLObject* toObject();
    inline FwMLArray* toArray();

    virtual QByteArray toUtf8() const = 0;

    inline FwMLNode* parent() const;

    void takeFromParent();

private:
    Type m_type;
    FwMLNode* m_parent;
};

////////////////////////////////////////////////////////////////////////////////

class FwMLString : public FwMLNode
{
    typedef FwMLNode BaseClass;
public:
    explicit FwMLString();
    explicit FwMLString(const QByteArray& value);
    FwMLString(const QByteArray &value, const QByteArray& attr, FwMLObject* parent);

    QByteArray value;

    QByteArray toUtf8() const;
};

////////////////////////////////////////////////////////////////////////////////

class FwMLNumber : public FwMLNode
{
    typedef FwMLNode BaseClass;

public:
    FwMLNumber();

    int intValue;
    quint32 uintValue;
    qreal realValue;

    QByteArray toUtf8() const;
};

////////////////////////////////////////////////////////////////////////////////

class FwMLObject : public FwMLNode
{
    typedef FwMLNode BaseClass;
public:

    friend class FwMLNode;

    FwMLObject();
    FwMLObject(const QByteArray& attrName, FwMLObject* parent);
    ~FwMLObject();

    FwMLNode* addAttribute(const QByteArray& name, FwMLNode* value, bool replace = true);
    inline FwMLString* addAttribute(const QByteArray& name, const QByteArray& value);
    inline FwMLString* addAttribute(const QByteArray& name, const QString& value);
    inline FwMLString* addAttribute(const QByteArray &name, const QUrl& url);
    inline FwMLNumber* addAttribute(const QByteArray& name, int value);
    inline FwMLNumber* addAttribute(const QByteArray &name, quint32 value);
    inline FwMLNumber* addAttribute(const QByteArray& name, qreal value);
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

class FwMLArray : public FwMLNode
{
    typedef FwMLNode BaseClass;
public:
    FwMLArray();
    ~FwMLArray();

    QVector<FwMLNode*> data;

    QByteArray toUtf8() const;
};

#include "fwml_inl.h"

#endif // FIREWORKS_ML_H
