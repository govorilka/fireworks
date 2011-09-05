#include "treemodel.h"

#include "fwcore/fwml.h"

TreeModel::TreeModel(QObject *parent) :
    BaseClass(parent),
    m_rootObject(new FwMLObject())
{
    QByteArray fwml = "\"Scene\" : {\n"
                      "\"background\" : \"123.png\", \n"
                      "\"size\" : { \n"
                      "\"width\" : \"100\", \n"
                      "\"height\" : \"100\" }, \n"
                      "array[-56, 1, 2.25, 3e-3, 5.] \n"
                      "}";

    m_rootObject->parse(fwml);
}

TreeModel::~TreeModel()
{
    delete m_rootObject;
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!parent.isValid() && row == 0 && column == 0)
    {
        return createIndex(row, column, static_cast<void*>(m_rootObject));
    }

    FwMLNode* parentNode = static_cast<FwMLNode*>(parent.internalPointer());
    if(parentNode)
    {
        FwMLNode* childNode = 0;
        switch(parentNode->type())
        {
        case FwMLNode::T_Object:
            {
                QList<FwMLNode*> children = static_cast<FwMLObject*>(parentNode)->toList();
                if(row >= 0 && row < children.size())
                {
                    childNode = children.at(row);
                }
            }
            break;

        case FwMLNode::T_Array:
            childNode = static_cast<FwMLArray*>(parentNode)->item(row);

        default:
            break;
        }

        if(childNode)
        {
            return createIndex(row, column, static_cast<void*>(childNode));
        }
    }

    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &child) const
{
    if(child.isValid())
    {
        FwMLNode* childNode = static_cast<FwMLNode*>(child.internalPointer());
        if(childNode && childNode->parent())
        {
            FwMLNode* parentNode = childNode->parent();
            return createIndex(nodeRow(parentNode), 0, static_cast<void*>(parentNode));
        }
    }
    return QModelIndex();
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    if(!parent.isValid())
    {
        return 1;
    }

    FwMLNode* node = static_cast<FwMLNode*>(parent.internalPointer());
    if(node)
    {
        switch(node->type())
        {
        case FwMLNode::T_Object:
            return static_cast<FwMLObject*>(node)->attributesCount();

        case FwMLNode::T_Array:
            return static_cast<FwMLArray*>(node)->size();

        default:
            break;
        }
    }

    return 0;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if(index.isValid())
    {
        FwMLNode* node = static_cast<FwMLNode*>(index.internalPointer());
        switch(role)
        {
        case Qt::DisplayRole:
            return QString::fromUtf8(node->name());

        default:
            break;
        }
    }
    return QVariant();
}

int TreeModel::nodeRow(FwMLNode* node) const
{
    FwMLNode* parentNode = node->parent();
    if(parentNode)
    {
        switch(parentNode->type())
        {
        case FwMLNode::T_Object:
            return static_cast<FwMLObject*>(parentNode)->toList().indexOf(node);

        case FwMLNode::T_Array:
            return static_cast<FwMLArray*>(parentNode)->indexOf(node);

        default:
            break;
        }
    }
    return 0;
}

void TreeModel::parseFile(const QString& fileName)
{
    m_rootObject->removeAttributes();
    QFile file(fileName);
    if(file.exists())
    {
        m_rootObject->parse(&file);
    }
    reset();
}
