#include <QtCore/qbuffer.h>
#include <QtCore/qiodevice.h>
#include <QtCore/qdebug.h>

#include "fwml.h"
#include "fwmlparser.h"

//Parse utils
namespace
{
    struct ParseData;

    enum CharType
    {
        C_AZ,      //Alpha (A..Z, a..z, _)
        C_Ee,      //Char 'E' and 'e'
        C_Uni,     //Unicode symbol

        C_Num,     //Numbers (0..9)
        C_Fra,     //Decimal point (.)
        C_Sig,     //Char '+' and '-'

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

/* 32 */  C_Sp,  C_Uni, C_Str, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni,
/* 40 */  C_Uni, C_Uni, C_Uni, C_Sig, C_Sep, C_Sig, C_Fra, C_Uni,
/* 48 */  C_Num, C_Num, C_Num, C_Num, C_Num, C_Num, C_Num, C_Num,
/* 56 */  C_Num, C_Num, C_Col, C_Sep, C_Uni, C_Uni, C_Uni, C_Uni,

/* 64 */  C_Uni, C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_Ee,  C_AZ,  C_AZ,
/* 72 */  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,
/* 80 */  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,
/* 88 */  C_AZ,  C_AZ,  C_AZ,  C_LSq, C_Esc, C_RSq, C_Uni, C_AZ,

/* 96 */  C_Uni, C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_Ee,  C_AZ,  C_AZ,
/* 104*/  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,
/* 112*/  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,
/* 120*/  C_AZ,  C_AZ,  C_AZ,  C_LCu, C_Uni, C_RCu, C_Uni, C_Err,

    };

    typedef void(*CommandFunc)(char c, ParseData* data);

    void x_doc(char c, ParseData* data) throw(Fw::Exception&);
    void x_var(char c, ParseData* data) throw(Fw::Exception&);
    void x_bst(char c, ParseData* data) throw(Fw::Exception&);
    void x_est(char c, ParseData* data) throw(Fw::Exception&);
    void x_atr(char c, ParseData* data) throw(Fw::Exception&);
    void x_int(char c, ParseData* data) throw(Fw::Exception&);
    void x_re1(char c, ParseData* data) throw(Fw::Exception&);
    void x_re2(char c, ParseData* data) throw(Fw::Exception&);
    void x_enu(char c, ParseData* data) throw(Fw::Exception&);
    void x_sg1(char c, ParseData* data) throw(Fw::Exception&);
    void x_rn3(char c, ParseData* data) throw(Fw::Exception&);
    void x_rs3(char c, ParseData* data) throw(Fw::Exception&);
    void x_err(char c, ParseData* data) throw(Fw::Exception&);
    void x_ob1(char c, ParseData* data) throw(Fw::Exception&);
    void x_ob2(char c, ParseData* data) throw(Fw::Exception&);
    void x_eob(char c, ParseData* data) throw(Fw::Exception&);
    void x_val(char c, ParseData* data) throw(Fw::Exception&);
    void x_ign(char c, ParseData* data) throw(Fw::Exception&);
    void x_ar1(char c, ParseData* data) throw(Fw::Exception&);
    void x_ar2(char c, ParseData* data) throw(Fw::Exception&);
    void x_ear(char c, ParseData* data) throw(Fw::Exception&);
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
        X_RE3 = 7,
        X_ATR = 8,
        X_SEO = 9,
        X_SEA = 10,
        X_EAT = 11,
        X_MAX = 12
    };

    //Parse command or parse state
    const CommandFunc parse_commands[X_MAX][C_MAX] = {
/*            C_AZ,   C_Ee,  C_Uni,  C_Num,  C_Fra, C_Sig,    C_Sp,  C_Str,  C_Esc,  C_Col,            C_LCu,  C_RCu,  C_LSq,  C_RSq,  C_Sep,  C_Err */
/*X_DOC*/{  &x_var, &x_var, &x_err, &x_err, &x_err, &x_err, &x_ign, &x_bst, &x_err, &x_err, /*X_DOC*/ &x_doc, &x_err, &x_err, &x_err, &x_err, &x_err  },
/*X_VAR*/{       0,      0, &x_err,      0, &x_err, &x_err, &x_est, &x_err, &x_err, &x_atr, /*X_VAR*/ &x_ob2, &x_eob, &x_ar2, &x_ear, &x_val, &x_err  },
/*X_STR*/{       0,      0,      0,      0,      0,      0,      0, &x_est, &x_err,      0, /*X_STR*/      0,      0,      0,      0,      0, &x_err  },
/*X_VAL*/{  &x_var, &x_var, &x_err, &x_int, &x_err, &x_sg1, &x_ign, &x_bst, &x_err, &x_err, /*X_VAL*/ &x_ob1, &x_err, &x_ar1, &x_ear, &x_val, &x_err  },
/*X_INT*/{  &x_err, &x_re2, &x_err,      0, &x_re1, &x_err, &x_enu, &x_err, &x_err, &x_err, /*X_INT*/ &x_err, &x_eob, &x_err, &x_ear, &x_val, &x_err  },
/*X_RE1*/{  &x_err, &x_re2, &x_err,      0, &x_err, &x_err, &x_enu, &x_err, &x_err, &x_err, /*X_RE1*/ &x_err, &x_eob, &x_err, &x_ear, &x_val, &x_err  },
/*X_RE2*/{  &x_err, &x_err, &x_err, &x_rn3, &x_err, &x_rn3, &x_err, &x_err, &x_err, &x_err, /*X_RE2*/ &x_err, &x_err, &x_err, &x_err, &x_err, &x_err  },
/*X_RE3*/{  &x_err, &x_err, &x_err,      0, &x_err, &x_err, &x_enu, &x_err, &x_err, &x_err, /*X_RE3*/ &x_err, &x_eob, &x_err, &x_ear, &x_val, &x_err  },
/*X_ATR*/{  &x_var, &x_var, &x_err, &x_err, &x_err, &x_err, &x_ign, &x_bst, &x_err, &x_err, /*X_ATR*/ &x_err, &x_eob, &x_err, &x_err, &x_err, &x_err  },
/*X_SEO*/{  &x_err, &x_err, &x_err, &x_err, &x_err, &x_err, &x_ign, &x_err, &x_err, &x_err, /*X_SEO*/ &x_err, &x_eob, &x_err, &x_err, &x_val, &x_err  },
/*X_SEA*/{  &x_err, &x_err, &x_err, &x_err, &x_err, &x_err, &x_ign, &x_err, &x_err, &x_err, /*X_SEA*/ &x_err, &x_err, &x_err, &x_ear, &x_val, &x_err  },
/*X_EAT*/{  &x_err, &x_err, &x_err, &x_err, &x_err, &x_err, &x_ign, &x_err, &x_err, &x_atr, /*X_EAT*/ &x_ob2, &x_err, &x_ar2, &x_err, &x_err, &x_err  },
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
        bool isVariable;
        int xcmd;
        int line;
        int column;
        quint32 uintNumber;
        quint8 charType;
        bool declareRoot;
        FwMLNode::Type type;
    };

    ParseData::ParseData() :
        parent(0),
        specialChar(false),
        isVariable(false),
        xcmd(X_DOC),
        line(0),
        column(0),
        uintNumber(0),
        charType(C_Err),
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
            {

                if(isVariable)
                {
                    bool bOk = false;
                    bool value = Fw::nameToBool(buffer, &bOk);
                    if(bOk)
                    {
                        new FwMLBool(value, attribute, static_cast<FwMLObject*>(parent));
                    }
                    else
                    {
                        new FwMLString(buffer, attribute, static_cast<FwMLObject*>(parent));
                    }
                }
                else
                {
                    new FwMLString(buffer, attribute, static_cast<FwMLObject*>(parent));
                }
                buffer = QByteArray();
            }
            break;

        case FwMLNode::T_UIntNumber:
            {
                bool bOk = false;
                quint32 value = buffer.toUInt(&bOk);
                if(!bOk)
                {
                    throw Fw::Exception("Invalid number value", "", line, column);
                }
                new FwMLUIntNumber(value, attribute, static_cast<FwMLObject*>(parent));
                buffer = QByteArray();
            }
            break;

        case FwMLNode::T_IntNumber:
            {
                bool bOk = false;
                int value = buffer.toInt(&bOk);
                if(!bOk)
                {
                    throw Fw::Exception("Invalid number value", line, column);
                }
                new FwMLIntNumber(value, attribute, static_cast<FwMLObject*>(parent));
                buffer = QByteArray();
            }
            break;

        case FwMLNode::T_DoubleNumber:
            {
                bool bOk = false;
                double value = buffer.toDouble(&bOk);
                if(!bOk)
                {
                    throw Fw::Exception("Invalid number value", line, column);
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
            {

                if(isVariable)
                {
                    bool bOk = false;
                    bool value = Fw::nameToBool(buffer, &bOk);
                    if(bOk)
                    {
                        new FwMLBool(value, static_cast<FwMLArray*>(parent));
                    }
                    else
                    {
                        new FwMLString(buffer, static_cast<FwMLArray*>(parent));
                    }
                }
                else
                {
                    new FwMLString(buffer, static_cast<FwMLArray*>(parent));
                }
                buffer = QByteArray();
            }
            break;

        case FwMLNode::T_UIntNumber:
            {
                bool bOk = false;
                quint32 value = buffer.toUInt(&bOk);
                if(!bOk)
                {
                    throw Fw::Exception("Invalid number value", line, column);
                }
                new FwMLUIntNumber(value, static_cast<FwMLArray*>(parent));
                buffer = QByteArray();
            }
            break;

        case FwMLNode::T_IntNumber:
            {
                bool bOk = false;
                int value = buffer.toInt(&bOk);
                if(!bOk)
                {
                    throw Fw::Exception("Invalid number value", line, column);
                }
                new FwMLIntNumber(value, static_cast<FwMLArray*>(parent));
                buffer = QByteArray();
            }
            break;

        case FwMLNode::T_DoubleNumber:
            {
                bool bOk = false;
                double value = buffer.toDouble(&bOk);
                if(!bOk)
                {
                    throw Fw::Exception("Invalid number value", line, column);
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

    void x_doc(char c, ParseData* data) throw(Fw::Exception&)
    {
        Q_UNUSED(c);
        data->xcmd = X_ATR;
        data->declareRoot = true;
    }

    void x_var(char c, ParseData* data) throw(Fw::Exception&)
    {
        if(data->parent)
        {
            data->type = FwMLNode::T_String;
            data->xcmd = X_VAR;
            data->isVariable = true;
            data->buffer += c;
            return;
        }
        throw Fw::Exception(c, data->line, data->column);
    }

    void x_bst(char c, ParseData* data) throw(Fw::Exception&)
    {
        Q_UNUSED(c);
        if(data->parent)
        {
            data->type = FwMLNode::T_String;
            data->xcmd = X_STR;
            data->isVariable = false;
            return;
        }
        throw Fw::Exception(c, data->line, data->column);
    }

    void x_est(char c, ParseData* data) throw(Fw::Exception&)
    {
        Q_UNUSED(c);
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

    void x_atr(char c, ParseData* data) throw(Fw::Exception&)
    {
        if(data->parent->type() != FwMLNode::T_Object)
        {
            throw Fw::Exception(c, data->line, data->column);
        }
        data->setupAttributeName();
        data->xcmd = X_VAL;
    }

    void x_int(char c, ParseData* data) throw(Fw::Exception&)
    {
        data->xcmd = X_INT;
        data->type = FwMLNode::T_UIntNumber;
        data->buffer += c;
    }

    void x_re1(char c, ParseData* data) throw(Fw::Exception&)
    {
        data->xcmd = X_RE1;
        data->type = FwMLNode::T_DoubleNumber;
        data->buffer += c;
    }

    void x_re2(char c, ParseData* data) throw(Fw::Exception&)
    {
        data->xcmd = X_RE2;
        data->type = FwMLNode::T_DoubleNumber;
        data->buffer += c;
    }

    void x_enu(char c, ParseData* data) throw(Fw::Exception&)
    {
        Q_UNUSED(c);
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

    void x_sg1(char c, ParseData* data) throw(Fw::Exception&)
    {
        if(c == '+')
        {
            data->xcmd = X_INT;
            data->type = FwMLNode::T_UIntNumber;
        }
        else if(c == '-')
        {
            data->xcmd = X_INT;
            data->type = FwMLNode::T_IntNumber;
            data->buffer += c;
        }
    }

    void x_rn3(char c, ParseData* data) throw(Fw::Exception&)
    {
        data->xcmd = X_RE3;
        data->buffer += c;
    }

    void x_ob1(char c, ParseData* data) throw(Fw::Exception&)
    {
        Q_UNUSED(c);
        data->type = FwMLNode::T_Object;
        data->setupValue();
        data->xcmd = X_ATR;
    }

    void x_ob2(char c, ParseData* data) throw(Fw::Exception&)
    {
        data->setupAttributeName();
        x_ob1(c,data);
    }

    void x_eob(char c, ParseData* data) throw(Fw::Exception&)
    {
        if(data->parent->type() == FwMLNode::T_Object)
        {
            data->structureUp();
            return;
        }
        throw Fw::Exception(c, data->line, data->column);
    }

    void x_ar1(char c, ParseData* data) throw(Fw::Exception&)
    {
        Q_UNUSED(c);
        data->type = FwMLNode::T_Array;
        data->setupValue();
        data->xcmd = X_VAL;
    }

    void x_ar2(char c, ParseData* data) throw(Fw::Exception&)
    {
        data->setupAttributeName();
        x_ar1(c, data);
    }

    void x_ear(char c, ParseData* data) throw(Fw::Exception&)
    {
        if(data->parent->type() == FwMLNode::T_Array)
        {
            data->structureUp();
            return;
        }
        throw Fw::Exception(c, data->line, data->column);
    }

    void x_err(char c, ParseData* data) throw(Fw::Exception&)
    {
        throw Fw::Exception(c, data->line, data->column);
    }

    void x_val(char c, ParseData* data) throw(Fw::Exception&)
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
        throw Fw::Exception(c, data->line, data->column);
    }

    void x_ign(char c, ParseData* data) throw(Fw::Exception&)
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
    parent->addNode(this);
}

FwMLNode::~FwMLNode()
{
    takeFromParent();
}

QByteArray FwMLNode::name() const
{
    if(m_parent)
    {
        switch(m_parent->type())
        {
        case FwMLNode::T_Object:
            return m_parent->cast<FwMLObject>()->attributeName(const_cast<FwMLNode*>(this));

        case FwMLNode::T_Array:
            return "[" + QByteArray::number(m_parent->cast<FwMLArray>()->indexOf(const_cast<FwMLNode*>(this))) + "]";

        default:
            break;
        }
    }
    else if(type() == FwMLNode::T_Object || type() == FwMLNode::T_Array)
    {
        return "root";
    }
    return "";
}

void FwMLNode::takeFromParent()
{
    if(m_parent)
    {
        switch(m_parent->type())
        {
        case FwMLNode::T_Object:
            {
                FwMLObject* object = static_cast<FwMLObject*>(m_parent);
                object->m_attributes.remove(object->m_attributes.key(this));
            }
            break;

        case FwMLNode::T_Array:
            {
                FwMLArray* array = static_cast<FwMLArray*>(m_parent);
                array->m_data.remove(array->m_data.indexOf(this));
            }
            break;

        default:
            Q_ASSERT(false);
            break;
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

int FwMLString::toInt(bool* bOk) const
{
    return m_value.toInt(bOk);
}

bool FwMLString::toBool(bool* bOk) const
{
    QByteArray lowerValue = m_value.toLower();
    if(lowerValue == "true" || lowerValue == "yes")
    {
        (*bOk) = true;
        return true;
    }
    (*bOk) = (lowerValue == "false" || lowerValue == "no");
    return false;
}

FwColor FwMLString::toColor(bool* bOk) const
{
    if(!m_value.isEmpty())
    {
        quint32 rgba = FwColor::nameToRGBA(m_value, bOk);
        if(*bOk)
        {
            return FwColor(rgba);
        }

        rgba = toUInt(bOk);
        if(*bOk)
        {
            return FwColor(rgba);
        }
    }

    (*bOk) = false;
    return FwColor();
}

quint32 FwMLString::toUInt(bool* bOk) const
{
    if(!m_value.isEmpty())
    {
        if(m_value.at(0) == '#')
        {
            return m_value.right(m_value.length() - 1).toUInt(bOk, 16);
        }
        return m_value.toUInt(bOk);
    }
    (*bOk) = false;
    return 0;
}

FwMLNode* FwMLString::clone() const
{
    return new FwMLString(m_value);
}


////////////////////////////////////////////////////////////////////////////////

FwMLUIntNumber::FwMLUIntNumber() :
    BaseClass(),
    m_value(0)
{
}

FwMLUIntNumber::FwMLUIntNumber(quint32 value) :
    BaseClass(),
    m_value(value)
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

int FwMLUIntNumber::toInt(bool* bOk) const
{
    if(m_value <= INT_MAX)
    {
        (*bOk) = true;
        return m_value;
    }
    (*bOk) = false;
    return 0;
}

bool FwMLUIntNumber::toBool(bool* bOk) const
{
   (*bOk) = true;
   return m_value == 0;
}

FwColor FwMLUIntNumber::toColor(bool* bOk) const
{
    (*bOk) = true;
    return FwColor(m_value);
}

FwMLNode* FwMLUIntNumber::clone() const
{
    FwMLUIntNumber* number = new FwMLUIntNumber();
    number->m_value = m_value;
    return number;
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

int FwMLIntNumber::toInt(bool* bOk) const
{
   (*bOk) = true;
   return m_value;
}

bool FwMLIntNumber::toBool(bool* bOk) const
{
   (*bOk) = true;
   return m_value == 0;
}

FwColor FwMLIntNumber::toColor(bool* bOk) const
{
    if(m_value < 0)
    {
        (*bOk) = false;
        return FwColor();
    }
    (*bOk) = true;
    return FwColor(m_value);
}

FwMLNode* FwMLIntNumber::clone() const
{
    FwMLIntNumber* number = new FwMLIntNumber();
    number->m_value = m_value;
    return number;
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

int FwMLDoubleNumber::toInt(bool* bOk) const
{
    if(qAbs(m_value) > 0. && (qAbs(m_value) - INT_MAX) < 0.)
    {
        (*bOk) = true;
        return m_value;
    }
    (*bOk) = false;
    return 0;
}

bool FwMLDoubleNumber::toBool(bool* bOk) const
{
   (*bOk) = true;
   return qFuzzyCompare(m_value, 0.);
}

FwColor FwMLDoubleNumber::toColor(bool* bOk) const
{
    (*bOk) = false;
    return FwColor();
}

FwMLNode* FwMLDoubleNumber::clone() const
{
    FwMLDoubleNumber* number = new FwMLDoubleNumber();
    number->m_value = m_value;
    return number;
}

////////////////////////////////////////////////////////////////////////////////

FwMLObject::FwMLObject() :
    BaseClass(),
    m_classObject(0)
{
}

FwMLObject::FwMLObject(const QByteArray& attrName, FwMLObject* parent) :
    BaseClass(attrName, parent),
    m_classObject(0)
{
}

FwMLObject::FwMLObject(FwMLArray* parent) :
   BaseClass(parent),
   m_classObject(0)
{
}

FwMLObject::~FwMLObject()
{
    removeAttributes();

    setClassObject(0);
    foreach(FwMLObject* object, m_inheritedObjects)
    {
        object->m_classObject = 0;
    }
}

void FwMLObject::addAttribute(const QByteArray& name, FwMLNode* value, bool replace)
{
    value->takeFromParent();

    FwMLNode* currentAttr = m_attributes.value(name);
    if(currentAttr)
    {
        if(replace)
        {
            delete currentAttr;
        }
        else
        {
            FwMLArray* addArray = currentAttr->cast<FwMLArray>();
            if(!addArray)
            {
                currentAttr->takeFromParent();
                addArray = new FwMLArray(name, this);
                addArray->addNode(currentAttr);
            }
            addArray->addNode(value);
            return;
        }
    }

    value->m_parent = this;
    m_attributes.insert(name, value);
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

void FwMLObject::parse(const QByteArray& utf8String) throw (Fw::Exception)
{
    if(utf8String.isEmpty())
    {
        throw Fw::Exception("Input string is empty");
    }
    QByteArray tmpStr = utf8String;
    QBuffer buffer(&tmpStr);
    parse(&buffer);
}

void FwMLObject::parse(QIODevice* ioDevice) throw (Fw::Exception)
{
    try
    {
        if(!ioDevice->isOpen() && !ioDevice->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            throw Fw::Exception(ioDevice->errorString());
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

        if(!data.buffer.isEmpty())
        {
            data.setupValue();
        }

    }
    catch(Fw::Exception& e)
    {
        removeAttributes();
        throw e;
    }
}

void FwMLObject::parseFile(const QString& fileName) throw(Fw::Exception)
{
    QFile file(QDir::toNativeSeparators(fileName));
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        throw Fw::Exception(file);
    }
    parse(&file);
}

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

int FwMLObject::toInt(bool* bOk) const
{
    (*bOk) = false;
    return 0;
}

bool FwMLObject::toBool(bool* bOk) const
{
   (*bOk) = false;
   return false;
}

FwColor FwMLObject::toColor(bool* bOk) const
{
    FwMLNode* name = attribute("name");
    if(name)
    {
        FwColor color = name->toColor(bOk);
        if(*bOk)
        {
            FwMLNode* blendingNode = attribute("blending");
            if(blendingNode)
            {
                bool blending = blendingNode->toBool(bOk);
                if(*bOk)
                {
                    color.setBlending(blending);
                    return color;
                }
            }
            else
            {
                return color;
            }
        }
    }
    (*bOk) = false;
    return FwColor();
}

FwMLNode* FwMLObject::clone() const
{
    FwMLObject* newObject = new FwMLObject();
    for(QHash<QByteArray, FwMLNode*>::const_iterator iter = m_attributes.begin(); iter != m_attributes.end(); ++iter)
    {
        FwMLNode* child = iter.value()->clone();
        child->m_parent = newObject;
        newObject->m_attributes.insert(iter.key(), child);
    }
    return newObject;
}

void FwMLObject::setClassObject(FwMLObject* object)
{
    if(m_classObject != object)
    {
        if(m_classObject)
        {
            m_classObject->m_inheritedObjects.remove(m_classObject->m_inheritedObjects.indexOf(this));
        }
        if(m_classObject = object)
        {
            m_classObject->m_inheritedObjects.append(this);
        }
    }
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
    foreach(FwMLNode* node, m_data)
    {
        Q_ASSERT(node->m_parent == this);
        node->m_parent = 0;
        delete node;
    }
    m_data.clear();
}

QByteArray FwMLArray::toUtf8() const
{
    QByteArray items;
    foreach(FwMLNode* node, m_data)
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

int FwMLArray::toInt(bool* bOk) const
{
    if(size() == 1)
    {
        return m_data.at(0)->toInt(bOk);
    }
    (*bOk) = false;
    return 0;
}

bool FwMLArray::toBool(bool* bOk) const
{
    if(size() == 1)
    {
        return m_data.at(0)->toBool(bOk);
    }
    (*bOk) = false;
    return false;
}

FwColor FwMLArray::toColor(bool* bOk) const
{
    if(size() == 1)
    {
        return m_data.at(0)->toColor(bOk);
    }
    (*bOk) = false;
    return false;
}

FwMLNode* FwMLArray::clone() const
{
    FwMLArray* newArray = new FwMLArray();
    newArray->m_data.reserve(m_data.size());
    foreach(FwMLNode* child, m_data)
    {
        newArray->addNode(child->clone());
    }
    return newArray;
}

void FwMLArray::addNode(FwMLNode* node)
{
    if(node->m_parent && node->m_parent != this)
    {
        node->takeFromParent();
    }
    node->m_parent = this;
    m_data.append(node);
}


///////////////////////////////////////////////////////////////////////////////

FwMLBool::FwMLBool() :
    BaseClass(),
    m_value(false)
{
}

FwMLBool::FwMLBool(bool value) :
    BaseClass(),
    m_value(value)
{
}

FwMLBool::FwMLBool(bool value, const QByteArray &attrName, FwMLObject *parent) :
    BaseClass(attrName, parent),
    m_value(value)
{
}

FwMLBool::FwMLBool(bool value, FwMLArray* parent) :
    BaseClass(parent),
    m_value(value)
{
}

QByteArray FwMLBool::toUtf8() const
{
    return m_value ? Fw::constantTrue : Fw::constantFalse;
}

int FwMLBool::toInt(bool *bOk) const
{
    (*bOk) = true;
    return m_value;
}

quint32 FwMLBool::toUInt(bool* bOk) const
{
    (*bOk) = true;
    return m_value;
}

FwMLNode* FwMLBool::clone() const
{
    FwMLBool* newNode = new FwMLBool();
    newNode->m_value = m_value;
    return newNode;
}

bool FwMLBool::toBool(bool *bOk) const
{
    (*bOk) = true;
    return m_value;
}

FwColor FwMLBool::toColor(bool *bOk) const
{
    (*bOk) = false;
    return FwColor();
}
