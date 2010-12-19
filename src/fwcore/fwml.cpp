#include <QtCore/qbuffer.h>
#include <QtCore/qiodevice.h>
#include <QtCore/qdebug.h>

#include "fwml.h"

namespace
{
    enum CharsType
    {
        C_Unknow = 0, //Unknow
        C_Alpha,      //Alpha (A..Z, a..z)
        C_QuMark,     //Quotation mark (")

    };

    const char chars_type[128] = {

          /* 0  */  /* 1  */  /* 2  */  /* 3  */  /* 4  */  /* 5  */  /* 6  */  /* 7  */
/*  0 */  C_Unknow, C_Unknow, C_Unknow, C_Unknow, C_Unknow, C_Unknow, C_Unknow, C_Unknow,
/*  8 */  C_Unknow, C_Unknow, C_Unknow, C_Unknow, C_Unknow, C_Unknow, C_Unknow, C_Unknow,
/* 16 */  C_Unknow, C_Unknow, C_Unknow, C_Unknow, C_Unknow, C_Unknow, C_Unknow, C_Unknow,
/* 24 */  C_Unknow, C_Unknow, C_Unknow, C_Unknow, C_Unknow, C_Unknow, C_Unknow, C_Unknow,

/* 32 */  C_Unknow, C_Unknow, C_QuMark, C_Unknow, C_Unknow, C_Unknow, C_Unknow, C_Unknow,
/* 40 */  C_Unknow, C_Unknow, C_Unknow, C_Unknow, C_Unknow, C_Unknow, C_Unknow, C_Unknow,
/* 48 */  C_Unknow, C_Unknow, C_Unknow, C_Unknow, C_Unknow, C_Unknow, C_Unknow, C_Unknow,
/* 56 */  C_Unknow, C_Unknow, C_Unknow, C_Unknow, C_Unknow, C_Unknow, C_Unknow, C_Unknow,

/* 64 */  C_Unknow, C_Alpha,  C_Alpha,  C_Alpha,  C_Alpha,  C_Alpha,  C_Alpha,  C_Alpha,
/* 72 */  C_Alpha,  C_Alpha,  C_Alpha,  C_Alpha,  C_Alpha,  C_Alpha,  C_Alpha,  C_Alpha,
/* 80 */  C_Alpha,  C_Alpha,  C_Alpha,  C_Alpha,  C_Alpha,  C_Alpha,  C_Alpha,  C_Alpha,
/* 88 */  C_Alpha,  C_Alpha,  C_Alpha,  C_Unknow, C_Unknow, C_Unknow, C_Unknow, C_Unknow,

/* 96 */  C_Unknow, C_Alpha,  C_Alpha,  C_Alpha,  C_Alpha,  C_Alpha,  C_Alpha,  C_Alpha,
/* 104*/  C_Alpha,  C_Alpha,  C_Alpha,  C_Alpha,  C_Alpha,  C_Alpha,  C_Alpha,  C_Alpha,
/* 112*/  C_Alpha,  C_Alpha,  C_Alpha,  C_Alpha,  C_Alpha,  C_Alpha,  C_Alpha,  C_Alpha,
/* 120*/  C_Alpha,  C_Alpha,  C_Alpha,  C_Unknow, C_Unknow, C_Unknow, C_Unknow, C_Unknow,

    };
}

////////////////////////////////////////////////////////////////////////////////

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

/*!
Разбирает данные, переданные в параметре utf8String, и строит по ним дерево.
\param utf8String Строка в формате FwML
\return Возвращает true, если данные разобраны успешно
\sa FwMLObject::parse(const QIODevice* ioDevice)
\note Если функцию передана пустая строка функция вернёт false
*/
bool FwMLObject::parse(const QByteArray& utf8String)
{
    if(!utf8String.isEmpty())
    {
        QByteArray tmpStr = utf8String;
        QBuffer buffer(&tmpStr);
        return parse(&buffer);
    }
    return false;
}

/*!
Разбирает данные прочитанные из QIODevice и строит по ним дерево.
\param ioDevice Указатель на QIODevice из которого будут прочитаны данные
\return Возвращает true, если данные разобраны успешно
\sa FwMLObject::parse(const QByteArray& utf8String)
*/
bool FwMLObject::parse(QIODevice* ioDevice)
{
    if(ioDevice->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while(!ioDevice->atEnd())
        {
            QByteArray line = ioDevice->readLine().trimmed();
            if(!line.isEmpty())
            {
                qDebug() << line;
                foreach(char c, line)
                {
                    if(c < 128)
                    {
                        char type = chars_type[c];
                        qDebug() << c << static_cast<int>(type);
                    }
                }
            }
        }
        return true;
    }
    return false;
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
