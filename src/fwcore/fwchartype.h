#ifndef FIREWORKS_CHARTYPE_H
#define FIREWORKS_CHARTYPE_H

#include <limits>

#include <QtCore/qbytearray.h>

#define CHARS_COUNT 256

namespace Fw
{
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

        C_Que,     //Question (?)

        //Error
        C_Err,     //Unknow

        C_MAX = C_Err + 1
    };

    extern const quint8 chars_type[CHARS_COUNT];

    typedef QByteArray Str;
    typedef Str::const_iterator StrIterator;
    inline static quint8 charType(const StrIterator& current);
}

quint8 Fw::charType(const StrIterator& current)
{
    return chars_type[static_cast<unsigned char>(*current)];
}

#endif //FIREWORKS_CHARTYPE_H
