#include "doublenumber.hpp"

Fw::ML::DoubleNumber::DoubleNumber(double value) :
    BaseClass(),
    m_value(value)
{
}

QByteArray Fw::ML::DoubleNumber::toUtf8() const
{
    return QByteArray::number(m_value);
}

int Fw::ML::DoubleNumber::toInt(bool* bOk) const
{
    if(qAbs(m_value) > 0. && (qAbs(m_value) - INT_MAX) < 0.)
    {
        (*bOk) = true;
        return m_value;
    }
    (*bOk) = false;
    return 0;
}

bool Fw::ML::DoubleNumber::toBool(bool* bOk) const
{
   (*bOk) = true;
   return qFuzzyCompare(m_value, 0.);
}

FwColor Fw::ML::DoubleNumber::toColor(bool* bOk) const
{
    (*bOk) = false;
    return FwColor();
}

Fw::ML::Node* Fw::ML::DoubleNumber::clone() const
{
    return new DoubleNumber(value());
}
