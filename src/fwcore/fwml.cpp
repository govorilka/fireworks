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
        FwMLParserException(const QString& err, ParseData* data);
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
/* 88 */  C_AZ,  C_AZ,  C_AZ,  C_LSq, C_Esc, C_RSq, C_Err, C_Err,

/* 96 */  C_Err, C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,
/* 104*/  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,
/* 112*/  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,
/* 120*/  C_AZ,  C_AZ,  C_AZ,  C_LCu, C_Err, C_RCu, C_Err, C_Err,

    };

    typedef void(*CommandFunc)(char c, ParseData* data);

    void x_doc(char c, ParseData* data) throw(FwMLParserException&);
    void x_var(char c, ParseData* data) throw(FwMLParserException&);
    void x_bst(char c, ParseData* data) throw(FwMLParserException&);
    void x_est(char c, ParseData* data) throw(FwMLParserException&);
    void x_atr(char c, ParseData* data) throw(FwMLParserException&);
    void x_int(char c, ParseData* data) throw(FwMLParserException&);
    void x_re1(char c, ParseData* data) throw(FwMLParserException&);
    void x_enu(char c, ParseData* data) throw(FwMLParserException&);
    void x_err(char c, ParseData* data) throw(FwMLParserException&);
    void x_ob1(char c, ParseData* data) throw(FwMLParserException&);
    void x_ob2(char c, ParseData* data) throw(FwMLParserException&);
    void x_eob(char c, ParseData* data) throw(FwMLParserException&);
    void x_val(char c, ParseData* data) throw(FwMLParserException&);
    void x_ign(char c, ParseData* data) throw(FwMLParserException&);
    void x_ar1(char c, ParseData* data) throw(FwMLParserException&);
    void x_ar2(char c, ParseData* data) throw(FwMLParserException&);
    void x_ear(char c, ParseData* data) throw(FwMLParserException&);
    /*void x_val(ParseData* data);

    void x_est(ParseData* data);
    void x_rnu(ParseData* data);
    void x_eob(ParseData* data);*/

    enum
    {
        X_DOC = 0,
        X_VAR = 1,
        X_STR = 2,
        X_VAL = 3,
        X_INT = 4,
        X_RE1 = 5,
        X_RE2 = 6,
        X_ATR = 7,
        X_SEO = 8,
        X_SEA = 9,
        X_EAT = 10,
        X_MAX = 11
    };

    //Parse command or parse state
    const CommandFunc parse_commands[X_MAX][C_MAX] = {
/*            C_AZ,  C_Uni,  C_Num,  C_Fra,   C_Sp,  C_Str,  C_Esc,  C_Col,            C_LCu,  C_RCu,  C_LSq,  C_RSq,  C_Sep,  C_Err */
/*X_DOC*/{  &x_var, &x_err, &x_err, &x_err, &x_ign, &x_bst, &x_err, &x_err, /*X_DOC*/ &x_doc, &x_err, &x_err, &x_err, &x_err, &x_err  },
/*X_VAR*/{       0, &x_err,      0, &x_err, &x_est, &x_err, &x_err, &x_atr, /*X_VAR*/ &x_ob2, &x_eob, &x_ar2, &x_ear, &x_val, &x_err  },
/*X_STR*/{       0,      0,      0,      0,      0, &x_est, &x_err,      0, /*X_STR*/      0,      0,      0,      0,      0, &x_err  },
/*X_VAL*/{  &x_var, &x_err, &x_int, &x_err, &x_ign, &x_bst, &x_err, &x_err, /*X_VAL*/ &x_ob1, &x_err, &x_ar1, &x_ear, &x_val, &x_err  },
/*X_INT*/{  &x_err, &x_err,      0, &x_re1, &x_enu, &x_err, &x_err, &x_err, /*X_INT*/ &x_err, &x_eob, &x_err, &x_ear, &x_val, &x_err  },
/*X_RE1*/{  &x_err, &x_err,      0, &x_err, &x_enu, &x_err, &x_err, &x_err, /*X_RE1*/ &x_err, &x_eob, &x_err, &x_ear, &x_val, &x_err  },
/*X_RE2*/{  &x_err, &x_err,      0, &x_err, &x_enu, &x_err, &x_err, &x_err, /*X_RE2*/ &x_err, &x_eob, &x_err, &x_ear, &x_val, &x_err  },
/*X_ATR*/{  &x_var, &x_err, &x_err, &x_err, &x_ign, &x_bst, &x_err, &x_err, /*X_ATR*/ &x_err, &x_eob, &x_err, &x_err, &x_err, &x_err  },
/*X_SEO*/{  &x_err, &x_err, &x_err, &x_err, &x_ign, &x_err, &x_err, &x_err, /*X_SEO*/ &x_err, &x_eob, &x_err, &x_err, &x_val, &x_err  },
/*X_SEA*/{  &x_err, &x_err, &x_err, &x_err, &x_ign, &x_err, &x_err, &x_err, /*X_SEA*/ &x_err, &x_err, &x_err, &x_ear, &x_val, &x_err  },
/*X_EAT*/{  &x_err, &x_err, &x_err, &x_err, &x_ign, &x_err, &x_err, &x_atr, /*X_EAT*/ &x_ob2, &x_err, &x_ar2, &x_err, &x_err, &x_err  },
    };

    struct ParseData
    {
        ParseData();

        inline void setupAttributeName();
        inline void structureUp();
        void setupValue();
        inline void setupAttributeValue();
        inline void setupArrayValue();


        FwMLNode* parent;

        QByteArray attribute;

        bool specialChar;
        QByteArray buffer;

        int xcmd;

        int line;
        int column;

        quint32 uintNumber;

        quint8 charType;

        bool declareRoot;

        FwMLNode::Type type;
    };

    ParseData::ParseData() :
        specialChar(false),
        xcmd(X_DOC),
        line(0),
        column(0),
        parent(0),
        charType(C_Err),
        uintNumber(0),
        declareRoot(false),
        type(FwMLNode::T_Null)
    {
    }

    void ParseData::setupAttributeName()
    {
        attribute = buffer;
        buffer = QByteArray();
    }

    void ParseData::structureUp()
    {
        setupValue();
        parent = parent->parent();
        switch(parent->type())
        {
        case FwMLNode::T_Array:
            xcmd = X_SEA;
            return;

        case FwMLObject::T_Object:
            xcmd = X_SEO;
            return;

        default:
            Q_ASSERT(false);
            break;
        }
    }

    void ParseData::setupValue()
    {
        switch(parent->type())
        {
        case FwMLNode::T_Array:
            setupArrayValue();
            return;

        case FwMLObject::T_Object:
            setupAttributeValue();
            return;

        default:
            Q_ASSERT(false);
            break;
        }
    }

    void ParseData::setupAttributeValue()
    {
        switch(type)
        {
        case FwMLNode::T_String:
            new FwMLString(buffer, attribute, static_cast<FwMLObject*>(parent));
            buffer = QByteArray();
            break;

        case FwMLNode::T_UIntNumber:
            {
                bool bOk = false;
                quint32 value = buffer.toUInt(&bOk);
                if(!bOk)
                {
                    throw FwMLParserException(QString("Invalid number value"), this);
                }
                new FwMLUIntNumber(value, attribute, static_cast<FwMLObject*>(parent));
                buffer = QByteArray();
            }
            break;

        case FwMLNode::T_DoubleNumber:
            {
                bool bOk = false;
                double value = buffer.toDouble(&bOk);
                if(!bOk)
                {
                    throw FwMLParserException(QString("Invalid number value"), this);
                }
                new FwMLDoubleNumber(value, attribute, static_cast<FwMLObject*>(parent));
                buffer = QByteArray();
            }
            break;

        case FwMLNode::T_Array:
            parent = new FwMLArray(attribute, static_cast<FwMLObject*>(parent));
            break;

        case FwMLNode::T_Object:
            parent = new FwMLObject(attribute, static_cast<FwMLObject*>(parent));
            break;

        case FwMLNode::T_Null:
            break;

        default:
            Q_ASSERT(false);
            return;
        }
        attribute = "";
        type = FwMLNode::T_Null;
    }

    void ParseData::setupArrayValue()
    {
        switch(type)
        {
        case FwMLNode::T_String:
            new FwMLString(buffer, static_cast<FwMLArray*>(parent));
            buffer = QByteArray();
            break;

        case FwMLNode::T_UIntNumber:
            {
                bool bOk = false;
                quint32 value = buffer.toUInt(&bOk);
                if(!bOk)
                {
                    throw FwMLParserException(QString("Invalid number value"), this);
                }
                new FwMLUIntNumber(value, static_cast<FwMLArray*>(parent));
                buffer = QByteArray();
            }
            break;

        case FwMLNode::T_DoubleNumber:
            {
                bool bOk = false;
                double value = buffer.toDouble(&bOk);
                if(!bOk)
                {
                    throw FwMLParserException(QString("Invalid number value"), this);
                }
                new FwMLDoubleNumber(value, static_cast<FwMLArray*>(parent));
                buffer = QByteArray();
            }
            break;

        case FwMLNode::T_Array:
            parent = new FwMLArray(static_cast<FwMLArray*>(parent));
            break;

        case FwMLNode::T_Object:
            parent = new FwMLObject(static_cast<FwMLArray*>(parent));
            break;

        case FwMLNode::T_Null:
            break;

        default:
            Q_ASSERT(false);
            return;
        }
        type = FwMLNode::T_Null;
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

    FwMLParserException::FwMLParserException(const QString& err, ParseData* data) :
        BaseClass()
    {
        message = QString("%1, line %2, column %3")
                  .arg(err)
                  .arg(data->line)
                  .arg(data->column);
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
        data->xcmd = X_ATR;
        data->declareRoot = true;
    }

    void x_var(char c, ParseData* data) throw(FwMLParserException&)
    {
        if(data->parent)
        {
            data->type = FwMLNode::T_String;
            data->xcmd = X_VAR;
            data->buffer += c;
            return;
        }
        throw FwMLParserException(c, data);
    }

    void x_bst(char c, ParseData* data) throw(FwMLParserException&)
    {
        if(data->parent)
        {
            data->type = FwMLNode::T_String;
            data->xcmd = X_STR;
            return;
        }
        throw FwMLParserException(c, data);
    }

    void x_est(char c, ParseData* data) throw(FwMLParserException&)
    {
        switch(data->parent->type())
        {
        case FwMLNode::T_Array:
            data->xcmd = X_SEA;
            return;

        case FwMLNode::T_Object:
            data->xcmd = data->attribute.isEmpty() ? X_EAT : X_SEO;
            return;

        default:
            Q_ASSERT(false);
            return;
        }
    }

    void x_atr(char c, ParseData* data) throw(FwMLParserException&)
    {
        if(data->parent->type() != FwMLNode::T_Object)
        {
            throw FwMLParserException(c, data);
        }
        data->setupAttributeName();
        data->xcmd = X_VAL;
    }

    void x_int(char c, ParseData* data) throw(FwMLParserException&)
    {
        data->xcmd = X_INT;
        data->type = FwMLNode::T_UIntNumber;
        data->buffer += c;
    }

    void x_re1(char c, ParseData* data) throw(FwMLParserException&)
    {
        data->xcmd = X_RE1;
        data->type = FwMLNode::T_DoubleNumber;
        data->buffer += c;
    }

    void x_enu(char c, ParseData* data) throw(FwMLParserException&)
    {
        switch(data->parent->type())
        {
        case FwMLNode::T_Array:
            data->xcmd = X_SEA;
            return;

        case FwMLNode::T_Object:
            data->xcmd = X_SEO;
            return;

        default:
            Q_ASSERT(false);
            return;
        }
    }

    void x_ob1(char c, ParseData* data) throw(FwMLParserException&)
    {
        data->type = FwMLNode::T_Object;
        data->setupValue();
        data->xcmd = X_ATR;
    }

    void x_ob2(char c, ParseData* data) throw(FwMLParserException&)
    {
        data->setupAttributeName();
        x_ob1(c,data);
    }

    void x_eob(char c, ParseData* data) throw(FwMLParserException&)
    {
        if(data->parent->type() == FwMLNode::T_Object)
        {
            data->structureUp();
            return;
        }
        throw FwMLParserException(c, data);
    }

    void x_ar1(char c, ParseData* data) throw(FwMLParserException&)
    {
        data->type = FwMLNode::T_Array;
        data->setupValue();
        data->xcmd = X_VAL;
    }

    void x_ar2(char c, ParseData* data) throw(FwMLParserException&)
    {
        data->setupAttributeName();
        x_ar1(c, data);
    }

    void x_ear(char c, ParseData* data) throw(FwMLParserException&)
    {
        if(data->parent->type() == FwMLNode::T_Array)
        {
            data->structureUp();
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
        if(data->parent)
        {
            data->setupValue();
            switch(data->parent->type())
            {
            case FwMLNode::T_Array:
                data->xcmd = X_VAL;
                break;

            case FwMLObject::T_Object:
                data->xcmd = X_ATR;
                break;

            default:
                Q_ASSERT(false);
                break;
            }
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

FwMLNode::FwMLNode() :
    m_parent(0)
{
}

FwMLNode::FwMLNode(const QByteArray& attrName, FwMLObject* parent) :
    m_parent(parent)
{
   parent->addAttribute(attrName, this);
}

FwMLNode::FwMLNode(FwMLArray* parent) :
    m_parent(parent)
{
    parent->data.append(this);
}

FwMLNode::~FwMLNode()
{
    takeFromParent();
}

void FwMLNode::takeFromParent()
{
    if(m_parent)
    {
        if(m_parent->type() == FwMLNode::T_Object)
        {
            FwMLObject* object = static_cast<FwMLObject*>(m_parent);
            object->m_attributes.remove(object->m_attributes.key(this));
        }
        else if(m_parent->type() == FwMLNode::T_Array)
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
    BaseClass(),
    m_value(QByteArray())
{
}

FwMLString::FwMLString(const QByteArray& value) :
   BaseClass(),
   m_value(value)
{
}

FwMLString::FwMLString(const QByteArray &value, const QByteArray& attr, FwMLObject* parent) :
    BaseClass(attr, parent),
    m_value(value)
{
}

FwMLString::FwMLString(const QByteArray &value, FwMLArray* parent) :
    BaseClass(parent),
    m_value(value)
{
}

QByteArray FwMLString::toUtf8() const
{
    return "\"" + m_value + "\"";
}

////////////////////////////////////////////////////////////////////////////////

FwMLUIntNumber::FwMLUIntNumber() :
    BaseClass(),
    m_value(0)
{
}

FwMLUIntNumber::FwMLUIntNumber(quint32 value, const QByteArray& attrName, FwMLObject* parent) :
   BaseClass(attrName, parent),
   m_value(value)
{
}

FwMLUIntNumber::FwMLUIntNumber(quint32 value, FwMLArray* parent) :
    BaseClass(parent),
    m_value(value)
{
}

QByteArray FwMLUIntNumber::toUtf8() const
{
    return QByteArray::number(m_value);
}

////////////////////////////////////////////////////////////////////////////////

FwMLIntNumber::FwMLIntNumber() :
    BaseClass(),
    m_value(0)
{
}

FwMLIntNumber::FwMLIntNumber(int value, const QByteArray& attrName, FwMLObject* parent) :
    BaseClass(attrName, parent),
    m_value(value)
{
}

FwMLIntNumber::FwMLIntNumber(int value, FwMLArray* parent) :
    BaseClass(parent),
    m_value(value)
{
}

QByteArray FwMLIntNumber::toUtf8() const
{
    return QByteArray::number(m_value);
}

////////////////////////////////////////////////////////////////////////////////

FwMLDoubleNumber::FwMLDoubleNumber() :
    BaseClass(),
    m_value(0.)
{
}

FwMLDoubleNumber::FwMLDoubleNumber(double value) :
    BaseClass(),
    m_value(value)
{
}

FwMLDoubleNumber::FwMLDoubleNumber(double value, const QByteArray& attrName, FwMLObject* parent) :
   BaseClass(attrName, parent),
   m_value(value)
{
}

FwMLDoubleNumber::FwMLDoubleNumber(double value, FwMLArray* parent) :
   BaseClass(parent),
   m_value(value)
{
}

QByteArray FwMLDoubleNumber::toUtf8() const
{
    return QByteArray::number(m_value);
}

////////////////////////////////////////////////////////////////////////////////

FwMLObject::FwMLObject() :
    BaseClass()
{
}

FwMLObject::FwMLObject(const QByteArray& attrName, FwMLObject* parent) :
    BaseClass(attrName, parent)
{
}

FwMLObject::FwMLObject(FwMLArray* parent) :
   BaseClass(parent)
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
            FwMLArray* addArray = m_attributes.value(name)->cast<FwMLArray>();
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
        data.parent = this;
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
    BaseClass()
{
}

FwMLArray::FwMLArray(const QByteArray& attrName, FwMLObject* parent) :
   BaseClass(attrName, parent)
{
}

FwMLArray::FwMLArray(FwMLArray* parent) :
    BaseClass(parent)
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
