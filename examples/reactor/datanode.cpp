#include "datanode.h"
#include "datatype.h"


DataNode::DataNode() :
    m_row(0),
    m_parent(0),
    m_type(0),
    m_key(0)
{
}

DataNode::DataNode(DataType* type, int key, const QString& caption) :
    m_row(0),
    m_parent(0),
    m_type(type),
    m_key(key),
    m_caption(caption)
{
}

DataNode::~DataNode()
{
    foreach(DataNode* node, m_children)
    {
        delete node;
    }
}

DataNode* DataNode::createChild(DataType* type, int key, const QString& caption)
{
    DataNode* newChild = new DataNode(type, key, nextChildCaption(caption));
    newChild->m_parent = this;
    m_children.append(newChild);
    newChild->m_row = m_children.size() - 1;
    return newChild;
}

void DataNode::deleteChild(DataNode* node)
{
    Q_ASSERT(node->m_parent == this);

    m_children.removeOne(node);
    delete node;

    int index = 0;
    foreach(DataNode* node, m_children)
    {
        node->m_row = index;
        ++index;
    }
}

DataNode* DataNode::childByCaption(const QString& caption) const
{
    foreach(DataNode* node, m_children)
    {
        if(node->caption() == caption)
        {
            return node;
        }
    }
    return 0;
}

QString DataNode::nextChildCaption(const QString& caption) const
{
    QString newCaption = caption;
    if(caption.indexOf("%1") != -1)
    {
        int index = 1;
        newCaption = caption.arg(index);
        while(childByCaption(newCaption))
        {
            newCaption = caption.arg(++index);
        }
    }
    return newCaption;
}

