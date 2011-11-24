#include "string.hpp"

Fw::ML::String::String() :
    BaseClass(),
    m_value(QByteArray())
{
}

Fw::ML::String::String(const QByteArray& value) :
   BaseClass(),
   m_value(value)
{
}

Fw::ML::String::String(const QByteArray &value, const QByteArray& attr, Object* parent) :
    BaseClass(attr, parent),
    m_value(value)
{
}

Fw::ML::String::String(const QByteArray &value, Array* parent) :
    BaseClass(parent),
    m_value(value)
{
}

QByteArray Fw::ML::String::toUtf8() const
{
    return "\"" + m_value + "\"";
}

int Fw::ML::String::toInt(bool* bOk) const
{
    return m_value.toInt(bOk);
}

bool Fw::ML::String::toBool(bool* bOk) const
{
    QByteArray lowerValue = m_value.toLower();
    if(lowerValue == "true" || lowerValue == "yes")
    {
        (*bOk) = true;
        return true;
    }
    (*bOk) = (lowerValue == "false" || lowerValue == "no");
    return false;
}

FwColor Fw::ML::String::toColor(bool* bOk) const
{
    if(!m_value.isEmpty())
    {
        quint32 rgba = FwColor::nameToRGBA(m_value, bOk);
        if(*bOk)
        {
            return FwColor(rgba);
        }

        rgba = toUInt(bOk);
        if(*bOk)
        {
            return FwColor(rgba);
        }
    }

    (*bOk) = false;
    return FwColor();
}

quint32 Fw::ML::String::toUInt(bool* bOk) const
{
    if(!m_value.isEmpty())
    {
        if(m_value.at(0) == '#')
        {
            return m_value.right(m_value.length() - 1).toUInt(bOk, 16);
        }
        return m_value.toUInt(bOk);
    }
    (*bOk) = false;
    return 0;
}

Fw::ML::Node* Fw::ML::String::clone() const
{
    return new String(m_value);
}
