#include "intnumber.hpp"

Fw::ML::IntNumber::IntNumber(int value) :
    BaseClass(),
    m_value(value)
{
}

QByteArray Fw::ML::IntNumber::toUtf8() const
{
    return QByteArray::number(m_value);
}

int Fw::ML::IntNumber::toInt(bool* bOk) const
{
   (*bOk) = true;
   return m_value;
}

bool Fw::ML::IntNumber::toBool(bool* bOk) const
{
   (*bOk) = true;
   return m_value == 0;
}

FwColor Fw::ML::IntNumber::toColor(bool* bOk) const
{
    if(m_value < 0)
    {
        (*bOk) = false;
        return FwColor();
    }
    (*bOk) = true;
    return FwColor(m_value);
}

Fw::ML::Node* Fw::ML::IntNumber::clone() const
{
    IntNumber* number = new IntNumber();
    number->m_value = m_value;
    return number;
}
