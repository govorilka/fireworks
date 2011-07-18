#include <QtCore/qdebug.h>

#include "fwmlparser.h"

//Types of ASCII chars
const quint8 FwMLParser::chars_type[128] = {

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

};

FwMLParser::FwMLParser() :
    m_stateFunction(&FwMLParser::parseObject),
    m_bufferType(BT_Empty)
{
}

bool FwMLParser::parse(FwMLObject* object, const QByteArray& fwml)
{
    return parseLine(fwml);
}

bool FwMLParser::parse(FwMLObject *object, QIODevice* ioDevice)
{

}

bool FwMLParser::parseLine(const QByteArray& line)
{
    if(!line.isEmpty())
    {
        const char* c = line.data();
        const char* endChar = c + line.size();
        while(c != endChar)
        {
            if(!((this->*FwMLParser::m_stateFunction)(c, endChar)))
            {
                return false;
            }
        }
    }
    return true;
}

bool FwMLParser::parseObject(str_interator& c, str_interator& endChar)
{
    ignoreSpace(c, endChar);
    switch(chars_type[*c])
    {
    case C_AZ:
        pushState(&FwMLParser::parseAttrValue);
        pushState(&FwMLParser::parseName);
        return true;
    }

    return false;
}

bool FwMLParser::parseName(str_interator& c, str_interator& endChar)
{
    m_bufferType = BT_Name;

    ignoreSpace(c, endChar);
    while(c != endChar)
    {
        if(*c > 127)
        {
            return false;
        }
        switch(chars_type[*c])
        {
        case C_AZ:
        case C_Num:
            m_buffer += (*c);
            ++c;
            break;

        default:
            popState();
            return true;
        }
    }

    popState();
    return true;
}

bool FwMLParser::parseAttrValue(str_interator& c, str_interator& endChar)
{
    qDebug() << "Attr name: " << m_buffer;
    while(c != endChar)
    {
        ++c;
    }
    return true;
}

