#ifndef FIREWORKS_ML_PARSER_H
#define FIREWORKS_ML_PARSER_H

#include "fireworks.h"

#include  <QtCore/qbytearray.h>
#include  <QtCore/qstack.h>

class QIODevice;

class FwMLObject;

class FIREWORKSSHARED_EXPORT FwMLParser
{
public:

    enum CharType
    {
        C_AZ,      //Alpha (A..Z, a..z)
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
    static const quint8 chars_type[128];

    FwMLParser();

    bool parse(FwMLObject* object, const QByteArray& fwml);
    bool parse(FwMLObject *object, QIODevice* ioDevice);

protected:

    bool parseLine(const QByteArray& line);

    typedef const char* str_interator;
    typedef bool(FwMLParser::*StateFunc)(str_interator& c, str_interator& endChar);

    inline void pushState(StateFunc function);
    inline bool popState();

    //State functions
    bool parseObject(str_interator& c, str_interator& endChar);
    bool parseName(str_interator& c, str_interator& endChar);
    bool parseAttrValue(str_interator& c, str_interator& endChar);

    inline void ignoreSpace(str_interator& c, str_interator& endChar);

private:
    int m_state;

    QStack<StateFunc> m_stateFunctionStack;
    StateFunc m_stateFunction;

    enum BufferType
    {
        BT_Empty,
        BT_Name,
        BT_String
    };
    BufferType m_bufferType;
    QByteArray m_buffer;
};

void FwMLParser::pushState(StateFunc function)
{
    if(m_stateFunction)
    {
        m_stateFunctionStack.push(m_stateFunction);
    }
    m_stateFunction = function;
}

bool FwMLParser::popState()
{
    if(!m_stateFunctionStack.isEmpty())
    {
        m_stateFunction = m_stateFunctionStack.pop();
        return true;
    }
    return false;
}

void FwMLParser::ignoreSpace(str_interator& c, str_interator& endChar)
{
    while(c != endChar)
    {
        if(*c > 128 || chars_type[*c] != C_Sp)
        {
            return;
        }
        ++c;
    }
}

#endif // FIREWORKS_ML_PARSER_H
