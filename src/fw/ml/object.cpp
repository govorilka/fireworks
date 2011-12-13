#include "object.hpp"
#include "array.hpp"
#include "string.hpp"

Fw::ML::Object::Object() :
    BaseClass(),
    m_classObject(0)
{
}

Fw::ML::Object::~Object()
{
    removeAttributes();

    setClassObject(0);
    foreach(Fw::ML::Object* object, m_inheritedObjects)
    {
        object->m_classObject = 0;
    }
}

void Fw::ML::Object::addAttribute(const QByteArray& name, Node* value, bool replace)
{
    value->takeFromParent();

    Node* currentAttr = m_attributes.value(name);
    if(currentAttr)
    {
        if(replace)
        {
            delete currentAttr;
        }
        else
        {
            Array* addArray = currentAttr->cast<Array>();
            if(!addArray)
            {
                currentAttr->takeFromParent();
                addArray = new Array();
                addAttribute(name, addArray);
                addArray->addNode(currentAttr);
            }
            addArray->addNode(value);
            return;
        }
    }

    value->m_parent = this;
    m_attributes.insert(name, value);
}

QByteArray Fw::ML::Object::toUtf8() const
{
    QByteArray attributes;
    for(NodeHash::const_iterator iter = m_attributes.begin(); iter != m_attributes.end(); ++iter)
    {
        if(!attributes.isEmpty())
        {
           attributes += ",";
        }
        Node* node = iter.value();
        QByteArray value = node->toUtf8();
        if(!value.isEmpty())
        {
            attributes += ("\"" + iter.key() + "\"");
            attributes += ":";
            attributes += value;
        }
    }

    if(attributes.isEmpty())
    {
        return "";
    }

    return "{" + attributes + "}";
}

void Fw::ML::Object::removeAttributes()
{
    foreach(Node* node, m_attributes.values())
    {
        Q_ASSERT(node->m_parent == this);
        node->m_parent = 0;
        delete node;
    }
    m_attributes.clear();
}

int Fw::ML::Object::toInt(bool* bOk) const
{
    (*bOk) = false;
    return 0;
}

bool Fw::ML::Object::toBool(bool* bOk) const
{
   (*bOk) = false;
   return false;
}

FwColor Fw::ML::Object::toColor(bool* bOk) const
{
    Node* name = attribute("name");
    if(name)
    {
        FwColor color = name->toColor(bOk);
        if(*bOk)
        {
            Node* blendingNode = attribute("blending");
            if(blendingNode)
            {
                bool blending = blendingNode->toBool(bOk);
                if(*bOk)
                {
                    color.setBlending(blending);
                    return color;
                }
            }
            else
            {
                return color;
            }
        }
    }
    (*bOk) = false;
    return FwColor();
}

Fw::ML::Node* Fw::ML::Object::clone() const
{
    Fw::ML::Object* newObject = new Fw::ML::Object();
    for(NodeHash::const_iterator iter = m_attributes.begin(); iter != m_attributes.end(); ++iter)
    {
        Node* child = iter.value()->clone();
        child->m_parent = newObject;
        newObject->m_attributes.insert(iter.key(), child);
    }
    return newObject;
}

void Fw::ML::Object::setClassObject(Fw::ML::Object* object)
{
    if(m_classObject != object)
    {
        if(m_classObject)
        {
            m_classObject->m_inheritedObjects.remove(m_classObject->m_inheritedObjects.indexOf(this));
        }

        m_classObject = object;
        if(m_classObject)
        {
            m_classObject->m_inheritedObjects.append(this);
        }

    }
}

QByteArray Fw::ML::Object::stringValue(const QByteArray& attrName) const
{
    String* classNode = attribute(attrName)->cast<String>();
    if(classNode)
    {
        return classNode->value();
    }
    return QByteArray();
}
