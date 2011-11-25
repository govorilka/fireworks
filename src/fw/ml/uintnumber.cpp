#include "uintnumber.hpp"

Fw::ML::UIntNumber::UIntNumber() :
    BaseClass(),
    m_value(0)
{
}

Fw::ML::UIntNumber::UIntNumber(quint32 value) :
    BaseClass(),
    m_value(value)
{
}

Fw::ML::UIntNumber::UIntNumber(quint32 value, const QByteArray& attrName, Object* parent) :
   BaseClass(attrName, parent),
   m_value(value)
{
}

Fw::ML::UIntNumber::UIntNumber(quint32 value, Array* parent) :
    BaseClass(parent),
    m_value(value)
{
}

QByteArray Fw::ML::UIntNumber::toUtf8() const
{
    return QByteArray::number(m_value);
}

int Fw::ML::UIntNumber::toInt(bool* bOk) const
{
    if(m_value <= INT_MAX)
    {
        (*bOk) = true;
        return m_value;
    }
    (*bOk) = false;
    return 0;
}

bool Fw::ML::UIntNumber::toBool(bool* bOk) const
{
   (*bOk) = true;
   return m_value == 0;
}

FwColor Fw::ML::UIntNumber::toColor(bool* bOk) const
{
    (*bOk) = true;
    return FwColor(m_value);
}

Fw::ML::Node* Fw::ML::UIntNumber::clone() const
{
    Fw::ML::UIntNumber* number = new Fw::ML::UIntNumber();
    number->m_value = m_value;
    return number;
}
