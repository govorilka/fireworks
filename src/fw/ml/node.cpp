#include "node.hpp"
#include "array.hpp"
#include "object.hpp"

Fw::ML::Node::Node() :
    m_parent(0)
{
}

Fw::ML::Node::Node(const QByteArray& attrName, Fw::ML::Object* parent) :
    m_parent(parent)
{
   parent->addAttribute(attrName, this);
}

Fw::ML::Node::Node(Fw::ML::Array* parent) :
    m_parent(parent)
{
    parent->addNode(this);
}

Fw::ML::Node::~Node()
{
    takeFromParent();
}

QByteArray Fw::ML::Node::name() const
{
    if(m_parent)
    {
        switch(m_parent->type())
        {
        case Fw::ML::T_Object:
            return m_parent->cast<Object>()->attributeName(const_cast<Node*>(this));

        case Fw::ML::T_Array:
            return "[" + QByteArray::number(m_parent->cast<Array>()->indexOf(const_cast<Node*>(this))) + "]";

        default:
            break;
        }
    }
    else if(type() == Fw::ML::T_Object || type() == Fw::ML::T_Array)
    {
        return "root";
    }
    return "";
}

void Fw::ML::Node::takeFromParent()
{
    if(m_parent)
    {
        switch(m_parent->type())
        {
        case T_Object:
            static_cast<Object*>(m_parent)->m_attributes.remove(static_cast<Object*>(m_parent)->m_attributes.key(this));
            break;

        case T_Array:
            static_cast<Array*>(m_parent)->m_data.remove(static_cast<Array*>(m_parent)->m_data.indexOf(this));
            break;

        default:
            Q_ASSERT(false);
            break;
        }

        m_parent = 0;
    }
}
