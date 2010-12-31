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
        C_Uni,     //Unicode symbol

        C_Num,     //Numbers (0..9)
        C_Fra,     //Decimal point (.)

        C_Sp,      //Space (' ')

        C_Str,     //Quotation mark (")
        C_Esc,     //Escape

        //These are the six structural characters
        C_Col,     //Name separator, colon (:)
        C_LCu,     //Begin-object, left curly bracket ({)
        C_RCu,     //End-object, right curly bracket (})
        C_LSq,     //Left square bracket ([)
        C_RSq,     //Right square bracket (])
        C_Sep,     //Items separator: Comma (,) or (;)

        //Error
        C_Err,     //Unknow

        C_MAX = C_Err + 1
    };

    //Types of ASCII chars
    const quint8 chars_type[128] = {

          /*0 */ /*1 */ /*2 */ /*3 */ /*4 */ /*5 */ /*6 */ /*7 */
/*  0 */  C_Err, C_Err, C_Err, C_Err, C_Err, C_Err, C_Err, C_Err,
/*  8 */  C_Err, C_Sp,  C_Err, C_Err, C_Err, C_Err, C_Err, C_Err,
/* 16 */  C_Err, C_Err, C_Err, C_Err, C_Err, C_Err, C_Err, C_Err,
/* 24 */  C_Err, C_Err, C_Err, C_Err, C_Err, C_Err, C_Err, C_Err,

/* 32 */  C_Sp,  C_Err, C_Str, C_Err, C_Err, C_Err, C_Err, C_Err,
/* 40 */  C_Err, C_Err, C_Err, C_Err, C_Sep, C_Err, C_Fra,  C_Err,
/* 48 */  C_Num, C_Num, C_Num, C_Num, C_Num, C_Num, C_Num, C_Num,
/* 56 */  C_Num, C_Num, C_Col, C_Sep, C_Err, C_Err, C_Err, C_Err,

/* 64 */  C_Err, C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,
/* 72 */  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,
/* 80 */  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,
/* 88 */  C_AZ,  C_AZ,  C_AZ,  C_LSq, C_Esc, C_LSq, C_Err, C_Err,

/* 96 */  C_Err, C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,
/* 104*/  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,
/* 112*/  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,
/* 120*/  C_AZ,  C_AZ,  C_AZ,  C_LCu, C_Err, C_RCu, C_Err, C_Err,

    };

    typedef void(*CommandFunc)(char c, ParseData* data);

    void x_doc(char c, ParseData* data) throw(FwMLParserException&);
    void x_var(char c, ParseData* data) throw(FwMLParserException&);
    void x_bst(char c, ParseData* data) throw(FwMLParserException&);
    void x_sep(char c, ParseData* data) throw(FwMLParserException&);
    void x_atr(char c, ParseData* data) throw(FwMLParserException&);
    void x_num(char c, ParseData* data) throw(FwMLParserException&);
    void x_err(char c, ParseData* data) throw(FwMLParserException&);
    void x_obj(char c, ParseData* data) throw(FwMLParserException&);
    void x_eob(char c, ParseData* data) throw(FwMLParserException&);
    void x_val(char c, ParseData* data) throw(FwMLParserException&);
    void x_ign(char c, ParseData* data) throw(FwMLParserException&);
    /*void x_val(ParseData* data);

    void x_est(ParseData* data);
    void x_rnu(ParseData* data);
    void x_eob(ParseData* data);*/

    enum
    {
        X_DOC = 0,
        X_VAR = 1,
        X_STR = 2,
        X_SEP = 3,
        X_VAL = 4,
        X_NUM = 5,
        X_OBJ = 6,
        X_ATR = 7,
        X_MAX = 8
    };

    //Parse command or parse state
    const CommandFunc parse_commands[X_MAX][C_MAX] = {
/*            C_AZ,  C_Uni,  C_Num,  C_Fra,   C_Sp,  C_Str,  C_Esc,  C_Col,            C_LCu,  C_RCu,  C_LSq,  C_RSq,  C_Sep,  C_Err */
/*X_DOC*/{  &x_var, &x_err, &x_err, &x_err, &x_ign, &x_bst, &x_err, &x_err, /*X_DOC*/ &x_doc, &x_err, &x_err, &x_err, &x_err, &x_err  },
/*X_VAR*/{       0, &x_err,      0, &x_err, &x_sep, &x_err, &x_err, &x_atr, /*X_VAR*/ &x_obj, &x_eob, &x_err, &x_err, &x_val, &x_err  },
/*X_STR*/{       0,      0,      0,      0,      0, &x_sep, &x_err,      0, /*X_STR*/      0,      0,      0,      0,      0, &x_err  },
/*X_SEP*/{  &x_err, &x_err, &x_err, &x_err, &x_ign, &x_err, &x_err, &x_atr, /*X_SEP*/ &x_obj, &x_eob, &x_err, &x_err, &x_val, &x_err  },
/*X_VAL*/{  &x_var, &x_err, &x_num, &x_err, &x_ign, &x_bst, &x_err, &x_atr, /*X_VAL*/ &x_obj, &x_err, &x_err, &x_err, &x_val, &x_err  },
/*X_NUM*/{  &x_err, &x_err, &x_num, &x_err, &x_sep, &x_err, &x_err, &x_err, /*X_NUM*/ &x_err, &x_eob, &x_err, &x_err, &x_val, &x_err  },
/*X_OBJ*/{  &x_var, &x_err, &x_err, &x_err, &x_ign, &x_bst, &x_err, &x_err, /*X_OBJ*/ &x_err, &x_eob, &x_err, &x_err, &x_err, &x_err  },
/*X_ATR*/{  &x_var, &x_err, &x_err, &x_err, &x_ign, &x_bst, &x_err, &x_err, /*X_ATR*/ &x_err, &x_err, &x_err, &x_err, &x_err, &x_err  },
    };

    struct ParseData
    {
        ParseData();

        FwMLObject* object;
        FwMLArray* array;

        QByteArray attribute;

        bool specialChar;
        QByteArray buffer;

        int xcmd;

        int line;
        int column;

        quint32 uintNumber;

        quint8 charType;

        bool declareRoot;

        FwMLString* stringNode;
    };

    ParseData::ParseData() :
        specialChar(false),
        xcmd(X_DOC),
        line(0),
        column(0),
        object(0),
        array(0),
        charType(C_Err),
        uintNumber(0),
        declareRoot(false),
        stringNode(0)
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

    void x_doc(char c, ParseData* data) throw(FwMLParserException&)
    {
        data->xcmd = X_OBJ;
        data->declareRoot = true;
    }

    void x_var(char c, ParseData* data) throw(FwMLParserException&)
    {
        data->xcmd = X_VAR;
        data->buffer += c;
    }

    void x_bst(char c, ParseData* data) throw(FwMLParserException&)
    {
        if(!data->stringNode)
        {
            if(!data->attribute.isEmpty())
            {
                data->stringNode = new FwMLString("", data->attribute, data->object);
                data->attribute = QByteArray();
            }
            data->xcmd = X_STR;
            return;
        }

        throw FwMLParserException(c, data);
    }

    void x_sep(char c, ParseData* data) throw(FwMLParserException&)
    {
        data->xcmd = X_SEP;
    }

    void x_atr(char c, ParseData* data) throw(FwMLParserException&)
    {
        if(data->object && !data->buffer.isEmpty() && data->attribute.isEmpty())
        {
            data->xcmd = X_VAL;
            data->attribute = data->buffer;
            data->buffer = QByteArray();
            return;
        }
        throw FwMLParserException(c, data);
    }

    void x_num(char c, ParseData* data) throw(FwMLParserException&)
    {
        data->xcmd = X_NUM;
        if(data->uintNumber > (UINT_MAX / 10))
        {
            throw FwMLParserException(c, data);
        }
        data->uintNumber = data->uintNumber * 10 + c - 48; //48 - '0'
    }

    void x_obj(char c, ParseData* data) throw(FwMLParserException&)
    {
        if(!data->buffer.isEmpty())
        {
            data->attribute = data->buffer;
            data->buffer = "";
        }
        else if(data->attribute.isEmpty())
        {
            throw FwMLParserException(c, data);
        }

        if(data->object)
        {
            data->xcmd = X_OBJ;
            data->object = new FwMLObject(data->attribute, data->object);
            data->attribute = "";
            return;
        }

        throw FwMLParserException(c, data);
    }

    void x_eob(char c, ParseData* data) throw(FwMLParserException&)
    {
        if(data->object)
        {
            x_val(c, data);

            FwMLNode* parent = data->object->parent();
            if(parent->type() == FwMLNode::T_Object)
            {
                data->object = parent->toObject();
                data->array = 0;
            }
            else if(parent->type() == FwMLNode::T_Array)
            {
                data->array = parent->toArray();
                data->object = 0;
            }
            else
            {
                Q_ASSERT(false);
            }

            data->xcmd = 0;

            return;
        }

        throw FwMLParserException(c, data);
    }

    void x_err(char c, ParseData* data) throw(FwMLParserException&)
    {
        throw FwMLParserException(c, data);
    }

    void x_val(char c, ParseData* data) throw(FwMLParserException&)
    {
        if(data->stringNode)
        {
            data->stringNode->value = data->buffer;
            data->stringNode = 0;
            data->buffer = QByteArray();
            data->xcmd = X_ATR;
            return;
        }
        throw FwMLParserException(c, data);
    }

    void x_ign(char c, ParseData* data) throw(FwMLParserException&)
    {
        Q_UNUSED(c);
        Q_UNUSED(data);
    }
}

////////////////////////////////////////////////////////////////////////////////

FwMLNode::FwMLNode(Type type) :
    m_type(type),
    m_parent(0)
{
}

FwMLNode::FwMLNode(Type type, const QByteArray& attrName, FwMLObject* parent) :
    m_type(type),
    m_parent(0)
{
   parent->addAttribute(attrName, this);
}

FwMLNode::~FwMLNode()
{
    takeFromParent();
}

void FwMLNode::takeFromParent()
{
    if(m_parent)
    {
        if(m_parent->m_type == FwMLNode::T_Object)
        {
            FwMLObject* object = static_cast<FwMLObject*>(m_parent);
            object->m_attributes.remove(object->m_attributes.key(this));
        }
        else if(m_parent->m_type == FwMLNode::T_Array)
        {
            FwMLArray* array = static_cast<FwMLArray*>(m_parent);
            array->data.remove(array->data.indexOf(this));
        }
        else
        {
            Q_ASSERT(false);
        }
        m_parent = 0;
    }
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

FwMLString::FwMLString(const QByteArray &str, const QByteArray& attr, FwMLObject* parent) :
    BaseClass(FwMLNode::T_String, attr, parent),
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

FwMLObject::FwMLObject(const QByteArray& attrName, FwMLObject* parent) :
    BaseClass(FwMLNode::T_Object, attrName, parent)
{
}

FwMLObject::~FwMLObject()
{
    removeAttributes();
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
    value->takeFromParent();
    value->m_parent = this;

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
bool FwMLObject::parse(const QByteArray& utf8String, QString* error)
{
    if(!utf8String.isEmpty())
    {
        QByteArray tmpStr = utf8String;
        QBuffer buffer(&tmpStr);
        return parse(&buffer, error);
    }
    return false;
}

/*!
Разбирает данные прочитанные из QIODevice и строит по ним дерево.
\param ioDevice Указатель на QIODevice из которого будут прочитаны данные
\return Возвращает true, если данные разобраны успешно
\sa FwMLObject::parse(const QByteArray& utf8String)
*/
bool FwMLObject::parse(QIODevice* ioDevice, QString* error)
{
    try
    {
        if(!ioDevice->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            throw FwMLParserException(ioDevice->errorString());
        }

        ParseData data;
        data.object = this;
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
                    data.charType = c > 0 ? chars_type[c] : C_Uni;
                    if(cmd = parse_commands[data.xcmd][data.charType])
                    {
                        cmd(c, &data);
                    }
                    else
                    {
                        data.buffer += c;
                    }
                }
            }
        }
    }
    catch(FwMLParserException& e)
    {
        qDebug() << e.what();
        if(error)
        {
            (*error) = e.message;
        }
        removeAttributes();
        return false;
    }

    return true;
}

/*!
Удаляет все атрибуты из объекта
*/
void FwMLObject::removeAttributes()
{
    foreach(FwMLNode* node, m_attributes.values())
    {
        Q_ASSERT(node->m_parent == this);
        node->m_parent = 0;
        delete node;
    }
    m_attributes.clear();
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
        Q_ASSERT(node->m_parent == this);
        node->m_parent = 0;
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
