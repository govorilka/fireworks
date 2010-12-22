#include <QtCore/qbuffer.h>
#include <QtCore/qiodevice.h>
#include <QtCore/qdebug.h>

#include "fwml.h"

//Parse utils
namespace
{
    struct ParseData;

    class FwMLParserException : public std::exception
    {
        typedef std::exception BaseClass;
    public:
        FwMLParserException(const QString& err);
        FwMLParserException(char c, ParseData* data);
        ~FwMLParserException() throw();

        const char* what() const throw();

        QString message;
    };

    enum CharType
    {
        C_AZ,      //Alpha (A..Z, a..z)
        C_Num,     //Numbers (0..9)
        C_Sp,      //Space (' ')
        C_Str,     //Quotation mark (")
        C_Col,     //Name separator, colon (:)
        C_LCu,     //Begin-object, left curly bracket ({)
        C_RCu,     //End-object, right curly bracket (})
        C_Poi,     //Decimal point (.)
        C_Uni,     //Unicode symbol
        C_Err,     //Unknow
        C_MAX = C_Err
    };

    //Types of ASCII chars
    const char chars_type[128] = {

          /*0 */ /*1 */ /*2 */ /*3 */ /*4 */ /*5 */ /*6 */ /*7 */
/*  0 */  C_Err, C_Err, C_Err, C_Err, C_Err, C_Err, C_Err, C_Err,
/*  8 */  C_Err, C_Sp,  C_Err, C_Err, C_Err, C_Err, C_Err, C_Err,
/* 16 */  C_Err, C_Err, C_Err, C_Err, C_Err, C_Err, C_Err, C_Err,
/* 24 */  C_Err, C_Err, C_Err, C_Err, C_Err, C_Err, C_Err, C_Err,

/* 32 */  C_Sp,  C_Err, C_Str, C_Err, C_Err, C_Err, C_Err, C_Err,
/* 40 */  C_Err, C_Err, C_Err, C_Err, C_Err, C_Err, C_Poi,  C_Err,
/* 48 */  C_Num, C_Num, C_Num, C_Num, C_Num, C_Num, C_Num, C_Num,
/* 56 */  C_Num, C_Num, C_Col, C_Err, C_Err, C_Err, C_Err, C_Err,

/* 64 */  C_Err, C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,
/* 72 */  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,
/* 80 */  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,
/* 88 */  C_AZ,  C_AZ,  C_AZ,  C_Err, C_Err, C_Err, C_Err, C_Err,

/* 96 */  C_Err, C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,
/* 104*/  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,
/* 112*/  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,
/* 120*/  C_AZ,  C_AZ,  C_AZ,  C_LCu, C_Err, C_RCu, C_Err, C_Err,

    };

    typedef void(*CommandFunc)(char c, ParseData* data);

    void x_var(char c, ParseData* data) throw(FwMLParserException&);
    void x_nva(char c, ParseData* data) throw(FwMLParserException&);
    void x_bob(char c, ParseData* data) throw(FwMLParserException&);
    void x_val(ParseData* data);
    void x_str(ParseData* data);
    void x_bst(ParseData* data);
    void x_est(ParseData* data);
    void x_num(ParseData* data);
    void x_rnu(ParseData* data);
    void x_err(ParseData* data);
    void x_obj(ParseData* data);
    void x_eob(ParseData* data);

    enum
    {
        X_VAR = 0,
        X_MAX = 2
    };

    //Parse command or parse state
    const CommandFunc parse_commands[X_MAX][C_MAX] = {
/*            C_AZ,  C_Num,   C_Sp,  C_Str,  C_Col,  C_LCu,  C_RCu,  C_Poi, C_Uni     */
/*X_VAR*/{  &x_var, &x_nva,      0,      0,      0, &x_bob,      0,      0,     0  },
    };

    struct ParseData
    {
        ParseData();

        QByteArray attribute;
        FwMLObject* object;

        bool specialChar;
        QByteArray string;
        int xcmd;

        int line;
        int column;
    };

    ParseData::ParseData() :
        specialChar(false),
        xcmd(X_VAR),
        line(0),
        column(0),
        object(0)
    {
    }


    FwMLParserException::FwMLParserException(char c, ParseData* data) :
        BaseClass()
    {
        message = QString("Unexcepted char '%1', line %2, column %3")
                  .arg(c)
                  .arg(data->line)
                  .arg(data->column);
    }

    FwMLParserException::FwMLParserException(const QString& err) :
        BaseClass(),
        message(err)
    {
    }

    const char* FwMLParserException::what() const throw()
    {
        return qPrintable(message);
    }

    FwMLParserException::~FwMLParserException() throw()
    {
    }

    void x_var(char c, ParseData* data) throw(FwMLParserException&)
    {
        data->xcmd = X_VAR;
        data->string += c;
    }

    void x_nva(char c, ParseData* data) throw(FwMLParserException&)
    {
        if(data->string.isEmpty())
        {
            throw FwMLParserException(c, data);
            return;
        }
        data->xcmd = X_VAR;
        data->string += c;
    }

    void x_bob(char c, ParseData* data) throw(FwMLParserException&)
    {
        if(!data->string.isEmpty())
        {
            data->attribute = data->string;
            data->string = "";
        }

        if(data->object && !data->attribute.isEmpty())
        {
            FwMLObject* newObject = new FwMLObject();

            data->xcmd = X_VAR;
            data->object->addAttribute(data->attribute, newObject);
            data->object = newObject;
            data->attribute = "";
            return;
        }

        throw FwMLParserException(c, data);
    }
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
    try
    {
        if(!ioDevice->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            throw FwMLParserException(ioDevice->errorString());
        }

        ParseData data;
        CommandFunc cmd = 0;
        while(!ioDevice->atEnd())
        {
            data.line++;
            data.column = 0;
            QByteArray line = ioDevice->readLine().trimmed();
            if(!line.isEmpty())
            {
                int line_size = line.size();
                char* c_ptr = line.data();
                for(;data.column < line_size; data.column++, c_ptr++)
                {
                    char& c = (*c_ptr);
                    char type = c > 0 ? chars_type[c] : C_Uni;
                    if(type != C_Err)
                    {
                        if(cmd = parse_commands[data.xcmd][type])
                        {
                            cmd(c, &data);
                        }
                    }
                }
            }
        }
    }
    catch(FwMLParserException& e)
    {
        qDebug() << e.what();
        return false;
    }

    return true;
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
