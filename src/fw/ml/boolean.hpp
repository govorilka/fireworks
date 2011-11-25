#ifndef FIREWORKS_ML_BOOLEAN_HPP
#define FIREWORKS_ML_BOOLEAN_HPP

#include "fw/ml/node.hpp"

class FIREWORKSSHARED_EXPORT Fw::ML::Boolean : public Fw::ML::BaseTempl<Fw::ML::T_Boolean>
{
    typedef Fw::ML::BaseTempl<Fw::ML::T_Boolean> BaseClass;

    bool m_value;

public:
    Boolean();
    Boolean(bool value);
    Boolean(bool value, const QByteArray& attrName, Object* parent);
    Boolean(bool value, Array* parent);

    inline bool value() const;
    inline void setValue(bool value);

    QByteArray toUtf8() const;

    int toInt(bool* bOk) const;
    bool toBool(bool *bOk) const;
    FwColor toColor(bool *bOk) const;
    quint32 toUInt(bool* bOk) const;

    Node* clone() const;
};

bool Fw::ML::Boolean::value() const
{
    return m_value;
}

void Fw::ML::Boolean::setValue(bool value)
{
    m_value = value;
}

#endif //FIREWORKS_ML_BOOLEAN_HPP
