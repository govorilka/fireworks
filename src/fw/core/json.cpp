#include "json.hpp"

#include <QtCore/qbuffer.h>
#include <QtCore/qiodevice.h>
#include <QtCore/qdebug.h>
#include <QtCore/qdir.h>

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
/*X_STR*/{       0,      0,      0,      0,      0,      0,      0, &x_est,      0,      0, /*X_STR*/      0,      0,      0,      0,      0, &x_err  },
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

        Fw::JSON::Node* parent;
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
        Fw::JSON::NodeType type;
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
        type(Fw::JSON::T_Null)
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
        if(parent)
        {
            switch(parent->type())
            {
            case Fw::JSON::T_Array:
                xcmd = X_SEA;
                return;

            case Fw::JSON::T_Object:
                xcmd = X_SEO;
                return;

            default:
                Q_ASSERT(false);
                break;
            }
        }
        else
        {
            xcmd = X_SEO;
        }
    }

    void ParseData::setupValue()
    {
        switch(parent->type())
        {
        case Fw::JSON::T_Array:
            setupArrayValue();
            return;

        case Fw::JSON::T_Object:
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
        case Fw::JSON::T_String:
            {
                if(isVariable)
                {
                    bool bOk = false;
                    bool value = Fw::JSON::nameToBool(buffer, &bOk);
                    if(bOk)
                    {
                        static_cast<Fw::JSON::Object*>(parent)->addBoolean(attribute, value);
                    }
                    else
                    {
                        static_cast<Fw::JSON::Object*>(parent)->addString(attribute, QString::fromUtf8(buffer));
                    }
                }
                else
                {
                    static_cast<Fw::JSON::Object*>(parent)->addString(attribute, QString::fromUtf8(buffer));
                }
                buffer = QByteArray();
            }
            break;

        case Fw::JSON::T_Number:
            {
                bool bOk = false;
                double value = buffer.toDouble(&bOk);
                if(!bOk)
                {
                    throw Fw::Exception("Invalid number value", line, column);
                }
                static_cast<Fw::JSON::Object*>(parent)->addNumber(attribute, value);
                buffer = QByteArray();
            }
            break;

        case Fw::JSON::T_Array:
            parent = static_cast<Fw::JSON::Object*>(parent)->addArray(attribute);
            break;

        case Fw::JSON::T_Object:
            parent = static_cast<Fw::JSON::Object*>(parent)->addObject(attribute);
            break;

        case Fw::JSON::T_Null:
            break;

        default:
            Q_ASSERT(false);
            return;
        }
        attribute = "";
        type = Fw::JSON::T_Null;
    }

    void ParseData::setupArrayValue()
    {
        switch(type)
        {
        case Fw::JSON::T_String:
            {
                if(isVariable)
                {
                    bool bOk = false;
                    bool value = Fw::JSON::nameToBool(buffer, &bOk);
                    if(bOk)
                    {
                        static_cast<Fw::JSON::Array*>(parent)->addBoolean(value);
                    }
                    else
                    {
                        static_cast<Fw::JSON::Array*>(parent)->addString(QString::fromUtf8(buffer));
                    }
                }
                else
                {
                    static_cast<Fw::JSON::Array*>(parent)->addString(QString::fromUtf8(buffer));
                }
                buffer = QByteArray();
            }
            break;

        case Fw::JSON::T_Number:
            {
                bool bOk = false;
                double value = buffer.toDouble(&bOk);
                if(!bOk)
                {
                    throw Fw::Exception("Invalid number value", line, column);
                }
                static_cast<Fw::JSON::Array*>(parent)->addNumber(value);
                buffer = QByteArray();
            }
            break;

        case Fw::JSON::T_Array:
            parent = static_cast<Fw::JSON::Array*>(parent)->addArray();
            break;

        case Fw::JSON::T_Object:
            parent = static_cast<Fw::JSON::Array*>(parent)->addObject();
            break;

        case Fw::JSON::T_Null:
            break;

        default:
            Q_ASSERT(false);
            return;
        }
        type = Fw::JSON::T_Null;
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
            data->type = Fw::JSON::T_String;
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
            data->type = Fw::JSON::T_String;
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
        case Fw::JSON::T_Array:
            data->xcmd = X_SEA;
            return;

        case Fw::JSON::T_Object:
            data->xcmd = data->attribute.isEmpty() ? X_EAT : X_SEO;
            return;

        default:
            Q_ASSERT(false);
            return;
        }
    }

    void x_atr(char c, ParseData* data) throw(Fw::Exception&)
    {
        if(data->parent->type() != Fw::JSON::T_Object)
        {
            throw Fw::Exception(c, data->line, data->column);
        }
        data->setupAttributeName();
        data->xcmd = X_VAL;
    }

    void x_int(char c, ParseData* data) throw(Fw::Exception&)
    {
        data->xcmd = X_INT;
        data->type = Fw::JSON::T_Number;
        data->buffer += c;
    }

    void x_re1(char c, ParseData* data) throw(Fw::Exception&)
    {
        data->xcmd = X_RE1;
        data->type = Fw::JSON::T_Number;
        data->buffer += c;
    }

    void x_re2(char c, ParseData* data) throw(Fw::Exception&)
    {
        data->xcmd = X_RE2;
        data->type = Fw::JSON::T_Number;
        data->buffer += c;
    }

    void x_enu(char c, ParseData* data) throw(Fw::Exception&)
    {
        Q_UNUSED(c);
        switch(data->parent->type())
        {
        case Fw::JSON::T_Array:
            data->xcmd = X_SEA;
            return;

        case Fw::JSON::T_Object:
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
            data->type = Fw::JSON::T_Number;
        }
        else if(c == '-')
        {
            data->xcmd = X_INT;
            data->type = Fw::JSON::T_Number;
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
        data->type = Fw::JSON::T_Object;
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
        if(data->parent->type() == Fw::JSON::T_Object)
        {
            data->structureUp();
            return;
        }
        throw Fw::Exception(c, data->line, data->column);
    }

    void x_ar1(char c, ParseData* data) throw(Fw::Exception&)
    {
        Q_UNUSED(c);
        data->type = Fw::JSON::T_Array;
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
        if(data->parent->type() == Fw::JSON::T_Array)
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
            case Fw::JSON::T_Array:
                data->xcmd = X_VAL;
                break;

            case Fw::JSON::T_Object:
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

bool Fw::JSON::nameToBool(const QByteArray& value, bool* bOk)
{
    if(bOk) (*bOk) = false;
    if(!value.isEmpty())
    {

        if(value == "true")
        {
            if(bOk) (*bOk) = true;
            return true;
        }
        else if(value == "false")
        {
            if(bOk) (*bOk) = true;
            return false;
        }
    }

    return false;
}

QByteArray Fw::JSON::boolToName(bool value)
{
    return value ? "true": "false";
}

////////////////////////////////////////////////////////////////////////////////

Fw::JSON::Node::Node() :
    m_parent(0)
{
}

Fw::JSON::Node::~Node()
{
    takeFromParent();
}

QByteArray Fw::JSON::Node::name() const
{
    if(m_parent)
    {
        switch(m_parent->type())
        {
        case Fw::JSON::T_Object:
            return m_parent->cast<Fw::JSON::Object>()->attributeName(const_cast<Fw::JSON::Node*>(this));

        case Fw::JSON::T_Array:
            return "[" + QByteArray::number(m_parent->cast<Fw::JSON::Array>()->indexOf(const_cast<Fw::JSON::Node*>(this))) + "]";

        default:
            break;
        }
    }
    else if(type() == Fw::JSON::T_Object || type() == Fw::JSON::T_Array)
    {
        return "root";
    }
    return "";
}

void Fw::JSON::Node::takeFromParent()
{
    if(m_parent)
    {
        switch(m_parent->type())
        {
        case Fw::JSON::T_Object:
            {
                Fw::JSON::Object* object = static_cast<Fw::JSON::Object*>(m_parent);
                object->m_attributes.remove(object->m_attributes.key(this));
            }
            break;

        case Fw::JSON::T_Array:
            {
                Fw::JSON::Array* array = static_cast<Fw::JSON::Array*>(m_parent);
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

Fw::JSON::String::String(const QString& value) :
   BaseClass(),
   m_value(value)
{
}

QByteArray Fw::JSON::String::toUtf8() const
{
    return "\"" + m_value.toUtf8() + "\"";
}

int Fw::JSON::String::toInt(bool* bOk) const
{
    return m_value.toInt(bOk);
}

uint Fw::JSON::String::toUint(bool* bOk) const
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

bool Fw::JSON::String::toBool(bool* bOk) const
{
    QString lowerValue = m_value.toLower();
    if(lowerValue == "true" || lowerValue == "yes")
    {
        (*bOk) = true;
        return true;
    }
    (*bOk) = (lowerValue == "false" || lowerValue == "no");
    return false;
}

double Fw::JSON::String::toNumber(bool* bOk) const
{
    return m_value.toDouble(bOk);
}

QString Fw::JSON::String::toString(bool* bOk) const
{
    QString out;
    out.reserve(m_value.size());

    QString::ConstIterator iter = m_value.begin();
    QString::ConstIterator end = m_value.end();
    try
    {
        while(iter != end)
        {
            if(*iter != QChar('\\'))
            {
                out.append(*iter);
                ++iter;
                continue;
            }

            if((++iter) == end)
            {
                throw QString();
            }

            switch(iter->unicode())
            {
            case 'b':
                out.append(QChar('\b'));
                break;

            case 'f':
                out.append(QChar('\f'));
                break;

            case 'n':
                out.append(QChar('\n'));
                break;

            case 'r':
                out.append(QChar('\r'));
                break;

            case 't':
                out.append(QChar('\t'));
                break;

            case '\\':
                out.append(QChar('\\'));
                break;

            case 'u':
                if((end - iter) >= 5)
                {
                    bool ok = false;
                    short uch = QString(iter+1, 4).toShort(&ok, 16);
                    if(!ok)
                    {
                        throw QString();
                    }

                    out.append(QChar(uch));

                    iter += 5;
                    continue;
                }
                break;

            default:
                throw QString();
            }

            ++iter;
        }
    }
    catch(const Fw::Exception& e)
    {
        (*bOk) = false;
        return QString();
    }

    (*bOk) = true;
    return out;
}

Fw::JSON::Node* Fw::JSON::String::clone() const
{
    return new Fw::JSON::String(m_value);
}

////////////////////////////////////////////////////////////////////////////////

Fw::JSON::Number::Number(double value) :
    BaseClass(),
    m_value(value)
{
}

QByteArray Fw::JSON::Number::toUtf8() const
{
    return QByteArray::number(m_value);
}

int Fw::JSON::Number::toInt(bool* bOk) const
{
    if(qAbs(m_value) > 0. && (qAbs(m_value) - INT_MAX) < 0.)
    {
        (*bOk) = true;
        return static_cast<int>(m_value);
    }
    (*bOk) = false;
    return 0;
}

uint Fw::JSON::Number::toUint(bool* bOk) const
{
    if(m_value > 0. && ((m_value - UINT_MAX) < 0.))
    {
        (*bOk) = true;
        return static_cast<uint>(m_value);
    }
    (*bOk) = false;
    return 0;
}

bool Fw::JSON::Number::toBool(bool* bOk) const
{
    (*bOk) = true;
    return qFuzzyCompare(m_value, 0.);
}

double Fw::JSON::Number::toNumber(bool* bOk) const
{
    return m_value;
}

QString Fw::JSON::Number::toString(bool* bOk) const
{
    return QString::number(m_value);
}

Fw::JSON::Node* Fw::JSON::Number::clone() const
{
    Fw::JSON::Number* number = new Fw::JSON::Number();
    number->m_value = m_value;
    return number;
}

///////////////////////////////////////////////////////////////////////////////

Fw::JSON::Boolean::Boolean(bool value) :
    BaseClass(),
    m_value(value)
{
}

QByteArray Fw::JSON::Boolean::toUtf8() const
{
    return m_value ? Fw::JSON::constantTrue : Fw::JSON::constantFalse;
}

int Fw::JSON::Boolean::toInt(bool* bOk) const
{
    (*bOk) = true;
    return m_value;
}

uint Fw::JSON::Boolean::toUint(bool* bOk) const
{
    (*bOk) = true;
    return m_value;
}

bool Fw::JSON::Boolean::toBool(bool* bOk) const
{
    (*bOk) = true;
    return m_value;
}

double Fw::JSON::Boolean::toNumber(bool* bOk) const
{
    (*bOk) = true;
    return m_value;
}

QString Fw::JSON::Boolean::toString(bool* bOk) const
{
    (*bOk) = true;
    return boolToName(m_value);
}

Fw::JSON::Node* Fw::JSON::Boolean::clone() const
{
    Fw::JSON::Boolean* newNode = new Fw::JSON::Boolean();
    newNode->m_value = m_value;
    return newNode;
}

////////////////////////////////////////////////////////////////////////////////

Fw::JSON::Object::Object() :
    BaseClass()
{
}

Fw::JSON::Object::~Object()
{
    clear();
}

void Fw::JSON::Object::clear()
{
    foreach(Fw::JSON::Node* node, m_attributes.values())
    {
        Q_ASSERT(node->m_parent == this);
        node->m_parent = 0;
        delete node;
    }
    m_attributes.clear();
}

Fw::JSON::Node* Fw::JSON::Object::addAttribute(const QByteArray& name, Fw::JSON::Node* value, bool replace)
{
    if(value->m_parent)
    {
        if(value->m_parent == this)
        {
            return value;
        }
        value->takeFromParent();
    }

    Fw::JSON::Node* currentAttr = m_attributes.value(name);
    if(currentAttr)
    {
        if(replace)
        {
            delete currentAttr;
        }
        else
        {
            Fw::JSON::Array* addArray = currentAttr->cast<Fw::JSON::Array>();
            if(!addArray)
            {
                addArray = this->addArray(name);
                addArray->addValue(currentAttr);
            }
            return addArray->addValue(value);
        }
    }

    value->m_parent = this;
    m_attributes.insert(name, value);
    return value;
}

QByteArray Fw::JSON::Object::toUtf8() const
{
    QByteArray attributes;
    for(QHash<QByteArray, Fw::JSON::Node*>::const_iterator iter = m_attributes.begin(); iter != m_attributes.end(); ++iter)
    {
        if(!attributes.isEmpty())
        {
           attributes += ",";
        }
        Fw::JSON::Node* node = iter.value();
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

void Fw::JSON::Object::parse(const QByteArray& utf8String) throw (Fw::Exception)
{
    if(utf8String.isEmpty())
    {
        throw Fw::Exception("Input string is empty");
    }
    QByteArray tmpStr = utf8String;
    QBuffer buffer(&tmpStr);
    parse(&buffer);
}

void Fw::JSON::Object::parse(QIODevice* ioDevice) throw (Fw::Exception)
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
        clear();
        throw e;
    }
}

void Fw::JSON::Object::parseFile(const QString& fileName) throw(Fw::Exception)
{
    QFile file(QDir::toNativeSeparators(fileName));
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        throw Fw::Exception(file);
    }
    parse(&file);
}

int Fw::JSON::Object::toInt(bool* bOk) const
{
    (*bOk) = false;
    return 0;
}

uint Fw::JSON::Object::toUint(bool* bOk) const
{
    (*bOk) = false;
    return 0;
}

bool Fw::JSON::Object::toBool(bool* bOk) const
{
    (*bOk) = false;
    return false;
}

double Fw::JSON::Object::toNumber(bool* bOk) const
{
    (*bOk) = false;
    return 0.;
}

QString Fw::JSON::Object::toString(bool* bOk) const
{
    (*bOk) = false;
    return QString();
}

Fw::JSON::Node* Fw::JSON::Object::clone() const
{
    Fw::JSON::Object* newObject = new Fw::JSON::Object();
    for(QHash<QByteArray, Fw::JSON::Node*>::const_iterator iter = m_attributes.begin(); iter != m_attributes.end(); ++iter)
    {
        Fw::JSON::Node* child = iter.value()->clone();
        child->m_parent = newObject;
        newObject->m_attributes.insert(iter.key(), child);
    }
    return newObject;
}

////////////////////////////////////////////////////////////////////////////////

Fw::JSON::Array::Array() :
    BaseClass()
{
}

Fw::JSON::Array::~Array()
{
    clear();
}

void Fw::JSON::Array::clear()
{
    foreach(Fw::JSON::Node* node, m_data)
    {
        Q_ASSERT(node->m_parent == this);
        node->m_parent = 0;
        delete node;
    }
    m_data.clear();
}

QByteArray Fw::JSON::Array::toUtf8() const
{
    QByteArray items;
    foreach(Fw::JSON::Node* node, m_data)
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

int Fw::JSON::Array::toInt(bool* bOk) const
{
    if(size() == 1)
    {
        return m_data.at(0)->toInt(bOk);
    }
    (*bOk) = false;
    return 0;
}

uint Fw::JSON::Array::toUint(bool* bOk) const
{
    if(size() == 1)
    {
        return m_data.at(0)->toUint(bOk);
    }
    (*bOk) = false;
    return 0;
}

bool Fw::JSON::Array::toBool(bool* bOk) const
{
    if(size() == 1)
    {
        return m_data.at(0)->toBool(bOk);
    }
    (*bOk) = false;
    return false;
}

double Fw::JSON::Array::toNumber(bool* bOk) const
{
    if(size() == 1)
    {
        return m_data.at(0)->toNumber(bOk);
    }
    (*bOk) = false;
    return 0.;
}

QString Fw::JSON::Array::toString(bool* bOk) const
{
    if(size() == 1)
    {
        return m_data.at(0)->toString(bOk);
    }
    (*bOk) = false;
    return QString();
}

Fw::JSON::Node* Fw::JSON::Array::clone() const
{
    Fw::JSON::Array* newArray = new Fw::JSON::Array();
    newArray->m_data.reserve(m_data.size());
    foreach(Fw::JSON::Node* child, m_data)
    {
        newArray->addValue(child->clone());
    }
    return newArray;
}

Fw::JSON::Node* Fw::JSON::Array::addValue(Fw::JSON::Node* node)
{
    if(node->m_parent)
    {
        if(node->m_parent == this)
        {
            return node;
        }
        node->takeFromParent();
    }
    node->m_parent = this;
    m_data.append(node);
    return node;
}
