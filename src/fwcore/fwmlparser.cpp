#include <QtCore/qdebug.h>
#include <QtCore/qbuffer.h>

#include "fwml.h"
#include "fwmlparser.h"

FwMLParser::FwMLParser() :
    m_bufferType(BT_Empty),
    m_lineIndex(0),
    m_currentNode(0),
    m_bufferUint(0),
    m_bufferNegative(false)
{
    clearBuffer();
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
            throw Fw::Exception(ioDevice->errorString());
        }

        object->removeAttributes();

        m_nodeStack.clear();
        m_currentNode = object;

        m_stateFunctionStack.clear();
        pushState(&FwMLParser::parseAttr);

        m_lineIndex = 0;

        Fw::StrIterator current = 0;
        Fw::StrIterator end = 0;
        while(!ioDevice->atEnd())
        {
            ++m_lineIndex;
            m_currentLine = ioDevice->readLine();
            current = m_currentLine.begin();
            end = m_currentLine.end();
            while(current != end)
            {
                if(Fw::charType(current) == Fw::C_Sp)
                {
                    ++current;
                }
                else
                {
                    popState(current, end);
                }
            }
        }
        while(!m_stateFunctionStack.isEmpty())
        {
            popState(current, end);
        }
    }
    catch(Fw::Exception& e)
    {
        qDebug() << e.what();
        m_errorString = e.error();
        return false;
    }

    return true;
}

void FwMLParser::parseObject(Fw::StrIterator& current, Fw::StrIterator& end) throw(Fw::Exception&)
{
    Q_UNUSED(end);
    switch(Fw::charType(current))
    {
    case Fw::C_AZ:
        pushState(&FwMLParser::parseAttrValue);
        pushState(&FwMLParser::parseName);
        return;
    }
}

void FwMLParser::parseAttr(Fw::StrIterator& current, Fw::StrIterator& end) throw(Fw::Exception&)
{
    Q_UNUSED(current);
    Q_UNUSED(end);
    pushState(&FwMLParser::parseAttrEnd);
    pushState(&FwMLParser::parseAttrValue);
    pushState(&FwMLParser::parseValue);
}

void FwMLParser::parseAttrValue(Fw::StrIterator& current, Fw::StrIterator& end) throw(Fw::Exception&)
{
    Q_UNUSED(end);

    switch(m_bufferType)
    {
    case BT_String:
    case BT_Name:
        m_attrName = m_buffer;
        if(m_attrName.isEmpty())
        {
            throw Fw::Exception("Attribute name is empty", m_lineIndex, column(current));
        }
        clearBuffer();
        break;

    default:
        throw Fw::Exception("Invalid attribute name", m_lineIndex, column(current));
        break;
    }

    switch(Fw::charType(current))
    {
    case Fw::C_Col:
        ++current;
        pushState(&FwMLParser::parseValue);
        break;

    default:
        throw Fw::Exception(*current, m_lineIndex, column(current));
        break;
    }
}

void FwMLParser::parseAttrEnd(Fw::StrIterator& current, Fw::StrIterator& end) throw(Fw::Exception&)
{  
    FwMLNode* child = createValue(current);
    if(m_attrName.isEmpty())
    {
        throw Fw::Exception("Attribute name is empty", m_lineIndex, column(current));
    }

    static_cast<FwMLObject*>(m_currentNode)->addAttribute(m_attrName, child);
    m_attrName = QByteArray();

    if(current != end)
    {
        switch(Fw::charType(current))
        {
        case Fw::C_Sep:
            pushState(&FwMLParser::parseAttr);
            ++current;
            return;

        default:
            break;
        }

        qDebug() << "FwMLParser::parseAttrValueEnd" << m_attrName << m_buffer;
        throw Fw::Exception(*current, m_lineIndex, column(current));
    }
}


void FwMLParser::parseValue(Fw::StrIterator& current, Fw::StrIterator& end) throw(Fw::Exception&)
{
    Q_UNUSED(end);

    clearBuffer();
    switch(Fw::charType(current))
    {
    case Fw::C_AZ:
    case Fw::C_Ee:
        m_bufferType = BT_Name;
        pushState(&FwMLParser::parseName);
        return;

    case Fw::C_Str:
        ++current; //Skip first quote
        m_bufferType = BT_String;
        pushState(&FwMLParser::parseString);
        return;

    case Fw::C_Num:
        m_bufferType = BT_UInt;
        pushState(&FwMLParser::parseUInt);
        return;

    case Fw::C_Fra:
        m_bufferType = BT_Real;
        pushState(&FwMLParser::parseDouble);
        return;

    default:
        throw Fw::Exception(*current, m_lineIndex, column(current));
        break;
    }    
}

void FwMLParser::parseName(Fw::StrIterator& current, Fw::StrIterator& end) throw(Fw::Exception&)
{
    while(current != end)
    {
        switch(Fw::charType(current))
        {
        case Fw::C_AZ:
        case Fw::C_Num:
        case Fw::C_Ee:
            m_buffer += (*current);
            ++current;
            break;

        case Fw::C_Uni:
        case Fw::C_Err:
            throw Fw::Exception(*current, m_lineIndex, column(current));
            return;

        default:
            return;
        }
    }
}

void FwMLParser::parseString(Fw::StrIterator& current, Fw::StrIterator& end) throw(Fw::Exception&)
{
    while(++current != end)
    {
        switch(Fw::charType(current))
        {
        case Fw::C_Str:
            ++current;
            return;

        default:
            m_buffer += (*current);
            break;
        }
    }
}

void FwMLParser::parseUInt(Fw::StrIterator& current, Fw::StrIterator& end) throw(Fw::Exception&)
{
    int digitCount = 0;
    int firstDigit = (*current) - '0';
    while(current != end)
    {
        switch(Fw::charType(current))
        {
        case Fw::C_Num:
            {
                int digit = (*current) - '0';
                if(m_bufferType)
                {
                    if(!firstDigit)
                    {
                        firstDigit = digit;
                    }
                    else
                    {
                        ++digitCount;
                        if((digitCount == 9 && firstDigit > 4) || (digitCount == 10))
                        {
                            throw Fw::Exception("Number is big",
                                                m_lineIndex,
                                                column(current));
                        }
                    }
                }
                m_bufferUint = m_bufferUint * 10 + digit;
            }
            break;

        case Fw::C_Fra:
        case Fw::C_Ee:
            pushState(&FwMLParser::parseDouble);
            return;

        default:
            return;
        }
        ++current;
    }
}

void FwMLParser::parseDouble(Fw::StrIterator& current, Fw::StrIterator& end) throw(Fw::Exception&)
{
    m_bufferType = BT_Real;
    m_buffer = QByteArray::number(m_bufferUint);
    m_bufferUint = 0;

    bool eAllow = true;
    bool sigAllow = false;
    bool fraAllow = true;
    while(current != end)
    {
        switch(Fw::charType(current))
        {
        case Fw::C_Num:
            m_buffer += (*current);
            sigAllow = false;
            ++current;
            break;

        case Fw::C_Fra:
            if(!fraAllow)
            {
                throw Fw::Exception(*current, m_lineIndex, column(current));
                return;
            }
            m_buffer += (*current);
            fraAllow = false;
            ++current;
            break;

        case Fw::C_Ee:
            if(!eAllow)
            {
                throw Fw::Exception(*current, m_lineIndex, column(current));
                return;
            }
            m_buffer += (*current);
            eAllow = false;
            sigAllow = true;
            ++current;
            break;

        case Fw::C_Sig:
            if(!sigAllow)
            {
                throw Fw::Exception(*current, m_lineIndex, column(current));
                return;
            }
            m_buffer += (*current);
            sigAllow = false;
            ++current;
            break;

        default:
            return;
        }
    }
}

FwMLNode* FwMLParser::createValue(Fw::StrIterator& current) throw(Fw::Exception&)
{
    if(!m_currentNode || m_bufferType == BT_Empty)
    {
        throw Fw::Exception("Syntax error",
                            m_lineIndex,
                            column(current));
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

        case BT_UInt:
            child = new FwMLUIntNumber(m_bufferUint);
            break;

        case BT_Real:
            {
                bool bOk = false;
                double value = m_buffer.toDouble(&bOk);
                if(!bOk)
                {
                    throw Fw::Exception("Invalid real number",
                                        m_lineIndex,
                                        column(current));
                }
                child = new FwMLDoubleNumber(value);
            }
            break;

        default:
            Q_ASSERT(false);
            return 0;
    }

    m_buffer = QByteArray();
    m_bufferUint = 0;
    m_bufferType = BT_Empty;
    return child;
}
