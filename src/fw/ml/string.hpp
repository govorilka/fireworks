#ifndef FW_ML_STRING_HPP
#define FW_ML_STRING_HPP

#include "fw/ml/node.hpp"

class FIREWORKSSHARED_EXPORT Fw::ML::String : public Fw::ML::BaseTempl<Fw::ML::T_String>
{
    typedef Fw::ML::BaseTempl<Fw::ML::T_String> BaseClass;

    QByteArray m_value;

public:
    explicit String();
    explicit String(const QByteArray& value);
    String(const QByteArray &value, const QByteArray& attr, Object* parent);
    String(const QByteArray &value, Array* parent);


    inline QByteArray value() const;
    inline void setValue(const QByteArray& value);

    inline bool isEmpty() const;

    int toInt(bool* bOk) const;
    bool toBool(bool* bOk) const;
    FwColor toColor(bool* bOk) const;
    quint32 toUInt(bool* bOk) const;
    QByteArray toUtf8() const;
    inline QString  toQString() const;

    Node* clone() const;
};

QByteArray Fw::ML::String::value() const
{
    return m_value;
}

void Fw::ML::String::setValue(const QByteArray& value)
{
    m_value = value;
}


bool Fw::ML::String::isEmpty() const
{
    return m_value.isEmpty();
}

QString Fw::ML::String::toQString() const
{
    return QString::fromUtf8(m_value);
}

#endif //FW_ML_STRING_HPP
