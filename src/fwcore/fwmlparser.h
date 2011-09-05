#ifndef FIREWORKS_ML_PARSER_H
#define FIREWORKS_ML_PARSER_H

#include "fireworks.h"

#include  <QtCore/qbytearray.h>
#include  <QtCore/qstack.h>

class QIODevice;

class FwMLNode;
class FwMLObject;

class FIREWORKSSHARED_EXPORT FwMLParserException : public std::exception
{
    typedef std::exception BaseClass;

public:
    FwMLParserException(const QString& err, int line = -1, int column = -1);
    FwMLParserException(char c, int line, int column);
    ~FwMLParserException() throw();

    const char* what() const throw();

    QString message;
};

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

    inline QString errorString() const;

protected:

    inline int column(const char* c) const;

    inline void pushNode(FwMLNode* node);
    inline bool popNode();

    typedef const char* str_interator;
    typedef void(FwMLParser::*StateFunc)(str_interator& c, str_interator& endChar);

    inline void pushState(StateFunc function);
    inline bool popState();

    //State functions
    void parseObject(str_interator& c, str_interator& endChar) throw(FwMLParserException&);

    void parseValue(str_interator& c, str_interator& endChar) throw(FwMLParserException&);

    void parseName(str_interator& c, str_interator& endChar) throw(FwMLParserException&);
    void parseString(str_interator& c, str_interator& endChar) throw(FwMLParserException&);

    void parseAttr(str_interator& c, str_interator& endChar) throw(FwMLParserException&);
    void parseAttrValue(str_interator& c, str_interator& endChar) throw(FwMLParserException&);
    void parseAttrValueEnd(str_interator& c, str_interator& endChar) throw(FwMLParserException&);

    inline void ignoreSpace(str_interator& c, str_interator& endChar);

    FwMLNode* createValue(str_interator& c) throw(FwMLParserException&);

private:

    QString m_errorString;

    int m_lineIndex;
    QByteArray m_currentLine;


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

    QByteArray m_attrName;

    QStack<FwMLNode*> m_nodeStack;
    FwMLNode* m_currentNode;
};

QString FwMLParser::errorString() const
{
    return m_errorString;
}

int FwMLParser::column(const char* c) const
{
    if(!m_currentLine.isEmpty())
    {
        return c - m_currentLine.data();
    }
    return 0;
}

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
    m_stateFunction = 0;
    return false;
}

void FwMLParser::pushNode(FwMLNode* node)
{
    if(m_currentNode)
    {
        m_nodeStack.push(m_currentNode);
    }
    m_currentNode = node;
}

bool FwMLParser::popNode()
{
    if(!m_nodeStack.isEmpty())
    {
        m_currentNode = m_nodeStack.pop();
        return true;
    }
    return false;
}

void FwMLParser::ignoreSpace(str_interator& c, str_interator& endChar)
{
    while(c != endChar && (*c) && chars_type[*c] == C_Sp)
    {
        ++c;
    }
}

#endif // FIREWORKS_ML_PARSER_H
