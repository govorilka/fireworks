#ifndef FIREWORKS_ML_PARSER_H
#define FIREWORKS_ML_PARSER_H

#include  <QtCore/qbytearray.h>
#include  <QtCore/qstack.h>

#include "fireworks.h"

#include "fwcore/fwchartype.h"

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

    FwMLParser();

    bool parse(FwMLObject* object, const QByteArray& fwml);
    bool parse(FwMLObject *object, QIODevice* ioDevice);

    inline QString errorString() const;

protected:

    inline int column(const char* c) const;

    inline void pushNode(FwMLNode* node);
    inline bool popNode();

    typedef void(FwMLParser::*StateFunc)(Fw::StrIterator& current, Fw::StrIterator& end);

    inline void pushState(StateFunc function);
    inline void popState(Fw::StrIterator& current, Fw::StrIterator& end) throw(FwMLParserException&);

    //State functions
    void parseObject(Fw::StrIterator& current, Fw::StrIterator& end) throw(FwMLParserException&);

    void parseValue(Fw::StrIterator& current, Fw::StrIterator& end) throw(FwMLParserException&);
    void parseName(Fw::StrIterator& current, Fw::StrIterator& end) throw(FwMLParserException&);
    void parseString(Fw::StrIterator& current, Fw::StrIterator& end) throw(FwMLParserException&);
    void parseUInt(Fw::StrIterator& current, Fw::StrIterator& end) throw(FwMLParserException&);
    void parseDouble(Fw::StrIterator& current, Fw::StrIterator& end) throw(FwMLParserException&);

    void parseAttr(Fw::StrIterator& current, Fw::StrIterator& end) throw(FwMLParserException&);
    void parseAttrValue(Fw::StrIterator& current, Fw::StrIterator& end) throw(FwMLParserException&);
    void parseAttrEnd(Fw::StrIterator& current, Fw::StrIterator& end) throw(FwMLParserException&);

    inline void clearBuffer();
    FwMLNode* createValue(Fw::StrIterator& current) throw(FwMLParserException&);

private:

    QString m_errorString;

    int m_lineIndex;
    QByteArray m_currentLine;

    QStack<StateFunc> m_stateFunctionStack;

    enum BufferType
    {
        BT_Empty,
        BT_Name,
        BT_String,
        BT_UInt,
        BT_Real
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
    m_stateFunctionStack.push(function);
}

void FwMLParser::popState(Fw::StrIterator& current, Fw::StrIterator& end) throw(FwMLParserException&)
{
    StateFunc state = m_stateFunctionStack.pop();
    (this->*state)(current, end);
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

void FwMLParser::clearBuffer()
{
    m_bufferType = BT_Empty;
    m_buffer = QByteArray();
    m_bufferUint = 0;
    m_bufferNegative = false;
}

#endif // FIREWORKS_ML_PARSER_H
