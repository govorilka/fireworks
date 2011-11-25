#ifndef FIREWORKS_ML_UINTNUMBER_H
#define FIREWORKS_ML_UINTNUMBER_H

#include "fw/ml/node.hpp"

class FIREWORKSSHARED_EXPORT Fw::ML::UIntNumber : public Fw::ML::BaseTempl<Fw::ML::T_UIntNumber>
{
    typedef Fw::ML::BaseTempl<Fw::ML::T_UIntNumber> BaseClass;

    quint32 m_value;

public:
    UIntNumber();
    UIntNumber(quint32 value);
    UIntNumber(quint32 value, const QByteArray& attrName, Object* parent);
    UIntNumber(quint32 value, Array* parent);

    inline quint32 value() const;
    inline void setValue(quint32 value);

    QByteArray toUtf8() const;

    int toInt(bool* bOk) const;
    bool toBool(bool* bOk) const;
    FwColor toColor(bool* bOk) const;

    Node* clone() const;
};

quint32 Fw::ML::UIntNumber::value() const
{
    return m_value;
}

void Fw::ML::UIntNumber::setValue(quint32 value)
{
    m_value = value;
}

#endif //FIREWORKS_ML_UINTNUMBER_H
