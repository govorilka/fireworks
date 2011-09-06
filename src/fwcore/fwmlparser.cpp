#include <QtCore/qdebug.h>
#include <QtCore/qbuffer.h>

#include "fwml.h"
#include "fwmlparser.h"

//Types of ASCII chars
const quint8 FwMLParser::chars_type[CHARS_COUNT] =
{
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
/* 88 */  C_AZ,  C_AZ,  C_AZ,  C_LSq, C_Esc, C_RSq, C_Uni, C_Uni,

/* 96 */  C_Uni, C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_Ee,  C_AZ,  C_AZ,
/* 104*/  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,
/* 112*/  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,  C_AZ,
/* 120*/  C_AZ,  C_AZ,  C_AZ,  C_LCu, C_Uni, C_RCu, C_Uni, C_Err,

/* 128*/  C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni,
/* 136*/  C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni,
/* 144*/  C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni,
/* 152*/  C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni,

/* 160*/  C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni,
/* 168*/  C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni,
/* 176*/  C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni,
/* 184*/  C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni,

/* 192*/  C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni,
/* 200*/  C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni,
/* 208*/  C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni,
/* 216*/  C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni,

/* 224*/  C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni,
/* 232*/  C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni,
/* 240*/  C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni,
/* 248*/  C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni, C_Uni
};

///////////////////////////////////////////////////////////////////////////////

FwMLParserException::FwMLParserException(const QString& err, int line, int column) :
    BaseClass()
{
    if(line != -1 && column != -1)
    {
        message = QString("%1, line %2, column %3")
                  .arg(err)
                  .arg(line)
                  .arg(column);
    }
    else
    {
        message = err;
    }

}

FwMLParserException::FwMLParserException(char c, int line, int column) :
    BaseClass()
{
    message = QString("Unexcepted char '%1', line %2, column %3")
              .arg(c)
              .arg(line)
              .arg(column);
}

const char* FwMLParserException::what() const throw()
{
    return qPrintable(message);
}

FwMLParserException::~FwMLParserException() throw()
{
}

///////////////////////////////////////////////////////////////////////////////

FwMLParser::FwMLParser() :
    m_bufferType(BT_Empty),
    m_lineIndex(0),
    m_stateFunction(0),
    m_currentNode(0)
{
}

bool FwMLParser::parse(FwMLObject* object, const QByteArray& fwml)
{
    if(!fwml.isEmpty())
    {
        QByteArray tmpStr = fwml;
        QBuffer buffer(&tmpStr);
        return parse(object, &buffer);
    }
    return false;
}

bool FwMLParser::parse(FwMLObject *object, QIODevice* ioDevice)
{
    m_errorString = QString();

    try
    {
        if(!ioDevice->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            throw FwMLParserException(ioDevice->errorString());
        }

        object->removeAttributes();

        m_nodeStack.clear();
        m_currentNode = object;

        m_stateFunctionStack.clear();
        m_stateFunction = &FwMLParser::parseAttr;

        m_lineIndex = 0;

        const char* c = 0;
        const char* endChar = 0;
        while(!ioDevice->atEnd())
        {
            ++m_lineIndex;
            m_currentLine = ioDevice->readLine().trimmed();
            if(!m_currentLine.isEmpty())
            {
                c = m_currentLine.data();
                endChar = c + m_currentLine.size();
                while(c != endChar)
                {
                    (this->*FwMLParser::m_stateFunction)(c, endChar);
                }
            }
        }

        while(m_stateFunction)
        {
            (this->*FwMLParser::m_stateFunction)(c, endChar);
            popState();
        }
    }
    catch(FwMLParserException& e)
    {
        qDebug() << e.what();
        m_errorString = e.message;
        return false;
    }

    return true;
}

void FwMLParser::parseObject(str_interator& c, str_interator& endChar) throw(FwMLParserException&)
{
    ignoreSpace(c, endChar);
    switch(chars_type[static_cast<unsigned char>(*c)])
    {
    case C_AZ:
        pushState(&FwMLParser::parseAttrValue);
        pushState(&FwMLParser::parseName);
        return;
    }
}

void FwMLParser::parseAttr(str_interator& c, str_interator& endChar) throw(FwMLParserException&)
{
    if(ignoreSpace(c, endChar) == endChar) return;

    switch(chars_type[static_cast<unsigned char>(*c)])
    {
    case C_AZ:
        popState();
        pushState(&FwMLParser::parseAttrValueEnd);
        pushState(&FwMLParser::parseAttrValue);
        pushState(&FwMLParser::parseName);
        return;

    default:
        break;
    }

    throw FwMLParserException(*c, m_lineIndex, column(c));
}

void FwMLParser::parseAttrValue(str_interator& c, str_interator& endChar) throw(FwMLParserException&)
{
    if(ignoreSpace(c, endChar) == endChar) return;

    switch(chars_type[static_cast<unsigned char>(*c)])
    {
    case C_Col:
        ++c;
        popState();
        m_attrName = m_buffer;
        pushState(&FwMLParser::parseValue);
        return;

    default:
        break;
    }

    throw FwMLParserException(*c, m_lineIndex, column(c));
}

void FwMLParser::parseAttrValueEnd(str_interator& c, str_interator& endChar) throw(FwMLParserException&)
{
    if(m_bufferType != BT_Empty)
    {
        FwMLNode* child = createValue(c);
        if(m_attrName.isEmpty())
        {
            throw FwMLParserException(QString("Attribute name is empty"), m_lineIndex, column(c));
            return;
        }

        static_cast<FwMLObject*>(m_currentNode)->addAttribute(m_attrName, child);
        m_attrName = QByteArray();
    }

    if(ignoreSpace(c, endChar) == endChar) return;

    switch(chars_type[static_cast<unsigned char>(*c)])
    {
    case C_Sep:
        popState();
        pushState(&FwMLParser::parseAttr);
        ++c;
        return;

    default:
        break;
    }

    qDebug() << "FwMLParser::parseAttrValueEnd" << m_attrName << m_buffer;
    throw FwMLParserException(*c, m_lineIndex, column(c));
}

void FwMLParser::parseValue(str_interator& c, str_interator& endChar) throw(FwMLParserException&)
{
    m_buffer = QByteArray();
    m_bufferType = BT_Empty;

    if(ignoreSpace(c, endChar) == endChar) return;

    switch(chars_type[static_cast<unsigned char>(*c)])
    {
    case C_AZ:
        popState();
        pushState(&FwMLParser::parseName);
        return;

    case C_Str:
        popState();
        pushState(&FwMLParser::parseString);
        return;

    default:
        break;
    }

    throw FwMLParserException(*c, m_lineIndex, column(c));
}

void FwMLParser::parseName(str_interator& c, str_interator& endChar) throw(FwMLParserException&)
{
    popState();
    m_bufferType = BT_Name;

    ignoreSpace(c, endChar);
    while(c != endChar)
    {
        switch(chars_type[static_cast<unsigned char>(*c)])
        {
        case C_AZ:
        case C_Num:
        case C_Ee:
            m_buffer += (*c);
            ++c;
            break;

        case C_Uni:
        case C_Err:
            throw FwMLParserException(*c, m_lineIndex, column(c));
            return;

        default:
            return;
        }
    }
}

void FwMLParser::parseString(str_interator& c, str_interator& endChar) throw(FwMLParserException&)
{
    if(chars_type[static_cast<unsigned char>(*c)] != C_Str)
    {
        throw FwMLParserException(*c, m_lineIndex, column(c));
        return;
    }

    m_bufferType = BT_String;

    while(++c != endChar)
    {
        const char & nextChar = *c;
        switch(chars_type[nextChar])
        {
        case C_Str:
            popState();
            ++c;
            return;

        default:
            m_buffer += nextChar;
            break;
        }
    }
}

FwMLNode* FwMLParser::createValue(str_interator& c) throw(FwMLParserException&)
{
    if(!m_currentNode || m_bufferType == BT_Empty)
    {
        throw FwMLParserException(QString("Syntax error"), m_lineIndex, column(c));
        return 0;
    }

    FwMLNode* child = 0;
    switch(m_bufferType)
    {
        case BT_String:
            child = new FwMLString(m_buffer);
            break;

        case BT_Name:
            {
                bool bOk = false;
                bool value = Fw::nameToBool(m_buffer, &bOk);
                if(bOk)
                {
                    child = new FwMLBool(value);
                }
                else
                {
                    child = new FwMLString(m_buffer);
                }
            }
            break;

        default:
            Q_ASSERT(false);
            return 0;
    }

    m_buffer = QByteArray();
    m_bufferType = BT_Empty;
    return child;
}
