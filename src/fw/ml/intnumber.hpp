#ifndef FIREWORKS_ML_INTNUMBER_H
#define FIREWORKS_ML_INTNUMBER_H

#include "fw/ml/node.hpp"

class FIREWORKSSHARED_EXPORT Fw::ML::IntNumber : public Fw::ML::BaseTempl<Fw::ML::T_IntNumber>
{
    typedef Fw::ML::BaseTempl<Fw::ML::T_IntNumber> BaseClass;

public:
    explicit IntNumber(int value = 0);

    QByteArray toUtf8() const;

    inline int value() const;
    inline void setValue(int value);

    int toInt(bool* bOk) const;
    bool toBool(bool* bOk) const;
    FwColor toColor(bool* bOk) const;

    Node* clone() const;

private:
   int m_value;
};

#endif //FIREWORKS_ML_INTNUMBER_H
