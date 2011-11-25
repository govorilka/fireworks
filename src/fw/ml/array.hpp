#ifndef FIREWORKS_ML_ARRAY_H
#define FIREWORKS_ML_ARRAY_H

#include <QtCore/qvector.h>

#include "fw/ml/node.hpp"

class FIREWORKSSHARED_EXPORT Fw::ML::Array : public Fw::ML::BaseTempl<Fw::ML::T_Array>
{
    typedef Fw::ML::BaseTempl<Fw::ML::T_Array> BaseClass;

    QVector<Node*> m_data;

public:

    friend class Node;

    Array();
    ~Array();

    void addNode(Node* node);

    inline int size() const;

    inline int indexOf(Node* item) const;
    inline Node* item(int index) const;

    QByteArray toUtf8() const;

    int toInt(bool* bOk) const;
    bool toBool(bool* bOk) const;
    FwColor toColor(bool* bOk) const;

    Node* clone() const;

    inline QVector<Node*> toVector() const;
};


int Fw::ML::Array::size() const
{
    return m_data.size();
}

int Fw::ML::Array::indexOf(Node* item) const
{
    return m_data.indexOf(item);
}

Fw::ML::Node* Fw::ML::Array::item(int index) const
{
    return (index < m_data.size() && index >= 0) ? m_data.at(index) : 0;
}

QVector<Fw::ML::Node*> Fw::ML::Array::toVector() const
{
    return m_data;
}

#endif //FIREWORKS_ML_ARRAY_H
