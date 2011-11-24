#include "array.hpp"

Fw::ML::Array::Array() :
    BaseClass()
{
}

Fw::ML::Array::Array(const QByteArray& attrName, Object* parent) :
   BaseClass(attrName, parent)
{
}

Fw::ML::Array::Array(Fw::ML::Array* parent) :
    BaseClass(parent)
{
}

Fw::ML::Array::~Array()
{
    foreach(Node* node, m_data)
    {
        Q_ASSERT(node->m_parent == this);
        node->m_parent = 0;
        delete node;
    }
    m_data.clear();
}

QByteArray Fw::ML::Array::toUtf8() const
{
    QByteArray items;
    foreach(Node* node, m_data)
    {
        if(!items.isEmpty())
        {
            items += ",";
        }
        items += node->toUtf8();
    }
    if(items.isEmpty())
    {
        return QByteArray();
    }
    return "[" + items + "]";
}

int Fw::ML::Array::toInt(bool* bOk) const
{
    if(size() == 1)
    {
        return m_data.at(0)->toInt(bOk);
    }
    (*bOk) = false;
    return 0;
}

bool Fw::ML::Array::toBool(bool* bOk) const
{
    if(size() == 1)
    {
        return m_data.at(0)->toBool(bOk);
    }
    (*bOk) = false;
    return false;
}

FwColor Fw::ML::Array::toColor(bool* bOk) const
{
    if(size() == 1)
    {
        return m_data.at(0)->toColor(bOk);
    }
    (*bOk) = false;
    return false;
}

Fw::ML::Node* Fw::ML::Array::clone() const
{
    Fw::ML::Array* newArray = new Fw::ML::Array();
    newArray->m_data.reserve(m_data.size());
    foreach(Fw::ML::Node* child, m_data)
    {
        newArray->addNode(child->clone());
    }
    return newArray;
}

void Fw::ML::Array::addNode(Node* node)
{
    if(node->m_parent && node->m_parent != this)
    {
        node->takeFromParent();
    }
    node->m_parent = this;
    m_data.append(node);
}
