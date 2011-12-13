#ifndef FIREWORKS_ML_DOUBLENUMBER_HPP
#define FIREWORKS_ML_DOUBLENUMBER_HPP

#include "fw/ml/node.hpp"

class FIREWORKSSHARED_EXPORT Fw::ML::DoubleNumber : public Fw::ML::BaseTempl<Fw::ML::T_DoubleNumber>
{
    typedef Fw::ML::BaseTempl<Fw::ML::T_DoubleNumber> BaseClass;

    double m_value;

public:
    explicit DoubleNumber(double value = 0.);

    inline double value() const;
    inline void setValue(double value);

    QByteArray toUtf8() const;

    int toInt(bool* bOk) const;
    bool toBool(bool* bOk) const;
    FwColor toColor(bool* bOk) const;

    Node* clone() const;
};

double Fw::ML::DoubleNumber::value() const
{
    return m_value;
}

void Fw::ML::DoubleNumber::setValue(double value)
{
    m_value = value;
}

#endif //FIREWORKS_ML_DOUBLENUMBER_HPP
