#ifndef FIREWORKS_ML_PARSER_H
#define FIREWORKS_ML_PARSER_H

#include  <QtCore/qbytearray.h>
#include  <QtCore/qstack.h>

#include <limits>

#include "fireworks.h"

#define CHARS_COUNT UCHAR_MAX + 1

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

////////////////////////////////////////////////////////////////////////////////

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

    static const quint8 chars_type[CHARS_COUNT];

    FwMLParser();

    bool parse(FwMLObject* object, const QByteArray& fwml);
    bool parse(FwMLObject *object, QIODevice* ioDevice);

    inline QString errorString() const;

protected:

    inline int column(const char* c) const;

    inline void pushNode(FwMLNode* node);
    inline bool popNode();

    typedef QByteArray Str;
    typedef Str::const_iterator StrIterator;
    inline static quint8 charType(const StrIterator& current);
    typedef void(FwMLParser::*StateFunc)(StrIterator& current, StrIterator& end);

    inline void pushState(StateFunc function);
    inline bool popState();

    //State functions
    void parseObject(StrIterator& current, StrIterator& end) throw(FwMLParserException&);

    void parseValue(StrIterator& current, StrIterator& end) throw(FwMLParserException&);
    void parseName(StrIterator& current, StrIterator& end) throw(FwMLParserException&);
    void parseString(StrIterator& current, StrIterator& end) throw(FwMLParserException&);
    void parseUInt(StrIterator& current, StrIterator& end) throw(FwMLParserException&);

    void parseAttr(StrIterator& current, StrIterator& end) throw(FwMLParserException&);
    void parseAttrValue(StrIterator& current, StrIterator& end) throw(FwMLParserException&);
    void parseAttrValueEnd(StrIterator& current, StrIterator& end) throw(FwMLParserException&);
    void parseAttrEnd(StrIterator& current, StrIterator& end) throw(FwMLParserException&);

    inline void clearBuffer();
    FwMLNode* createValue(StrIterator& current) throw(FwMLParserException&);

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
        BT_String,
        BT_UInt,
    };

    BufferType m_bufferType;
    QByteArray m_buffer;
    quint32 m_bufferUint;
    bool m_bufferNegative;

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

quint8 FwMLParser::charType(const StrIterator& current)
{
    return chars_type[static_cast<unsigned char>(*current)];
}

void FwMLParser::clearBuffer()
{
    m_bufferType = BT_Empty;
    m_buffer = QByteArray();
    m_bufferUint = 0;
    m_bufferNegative = false;
}

#endif // FIREWORKS_ML_PARSER_H
