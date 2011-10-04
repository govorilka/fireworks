#include "testdocumentmodel.h"
#include "testdocument.h"

Test::DocumentModelType::DocumentModelType(DocumentModel* model) :
    m_model(model)
{
}

Test::DocumentModelType::~DocumentModelType()
{
}

///////////////////////////////////////////////////////////////////////////////

Test::DocumentModelTypeRoot::DocumentModelTypeRoot(DocumentModel* model) :
    BaseClass(model),
    m_icon(":/reactor/images/datatree/database.png")
{
}

Test::NodeType Test::DocumentModelTypeRoot::typeID() const
{
    return Test::NT_Root;
}

QIcon Test::DocumentModelTypeRoot::icon(DocumentModelNode* node) const
{
    Q_UNUSED(node);
    return m_icon;
}

QString Test::DocumentModelTypeRoot::caption(DocumentModelNode* node) const
{
    return "root";
}

bool Test::DocumentModelTypeRoot::hasChildren() const
{
    return true;
}

void Test::DocumentModelTypeRoot::loadChildren(DocumentModelNode* parentNode) const
{
}

void Test::DocumentModelTypeRoot::createChild(DocumentModelNode* parentNode)
{
}

void Test::DocumentModelTypeRoot::removeNode(DocumentModelNode* node)
{
}

///////////////////////////////////////////////////////////////////////////////

Test::DocumentModelTypeQuestion::DocumentModelTypeQuestion(DocumentModel* model) :
    BaseClass(model)
{
}

Test::NodeType Test::DocumentModelTypeQuestion::typeID() const
{
    return Test::NT_Question;
}

QIcon Test::DocumentModelTypeQuestion::icon(DocumentModelNode* node) const
{
    return QIcon();
}

QString Test::DocumentModelTypeQuestion::caption(DocumentModelNode* node) const
{
    return QString();
}

bool Test::DocumentModelTypeQuestion::hasChildren() const
{
    return true;
}

void Test::DocumentModelTypeQuestion::loadChildren(DocumentModelNode* parentNode) const
{
}

void Test::DocumentModelTypeQuestion::createChild(DocumentModelNode* parentNode)
{
}

void Test::DocumentModelTypeQuestion::removeNode(DocumentModelNode* node)
{
}

////////////////////////////////////////////////////////////////////////////////

Test::DocumentModelNode::DocumentModelNode() :
    m_type(0),
    m_parent(0),
    m_key(0)
{
}

Test::DocumentModelNode::DocumentModelNode(DocumentModelType* type, DocumentModelNode* parent, int key) :
    m_type(type),
    m_parent(parent),
    m_key(key)
{
}

Test::DocumentModelNode::~DocumentModelNode()
{
    foreach(DocumentModelNode* node, m_children)
    {
        delete node;
    }
}

////////////////////////////////////////////////////////////////////////////////

Test::DocumentModel::DocumentModel(QObject* parent) :
    BaseClass(parent),
    m_document(0),
    m_rootNode(0),
    m_typeRoot(0),
    m_typeQuestion(0)
{
    m_document = new Document(this);
    m_typeRoot = new DocumentModelTypeRoot(this);
    m_typeQuestion = new DocumentModelTypeQuestion(this);
}

Test::DocumentModel::~DocumentModel()
{
    delete m_typeRoot;
    delete m_typeQuestion;
}

bool Test::DocumentModel::open(const QString& fileName, QString* errorMessage)
{
    delete m_rootNode;
    m_rootNode = 0;

    bool result = m_document->open(fileName, errorMessage);
    if(result)
    {
        m_rootNode = new DocumentModelNode(m_typeRoot, 0, 0);
        m_typeRoot->loadChildren(m_rootNode);
    }

    reset();
    return result;
}

void Test::DocumentModel::close()
{
    m_document->close();
    reset();
}

QModelIndex Test::DocumentModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(column);

    DocumentModelNode* dataNode = node(parent);
    if(dataNode)
    {
        return createIndex(dataNode->children().at(row));
    }

    return m_rootNode ? createIndex(m_rootNode) : QModelIndex();
}

QModelIndex Test::DocumentModel::parent(const QModelIndex &child) const
{
    DocumentModelNode* dataNode = node(child);
    if(dataNode && dataNode->parent())
    {
        return createIndex(dataNode->parent());
    }
    return QModelIndex();
}

int Test::DocumentModel::rowCount(const QModelIndex &parent) const
{
    DocumentModelNode* dataNode = node(parent);
    if(dataNode)
    {
        if(dataNode->type()->hasChildren())
        {
            if(dataNode->children().isEmpty())
            {
                dataNode->type()->loadChildren(dataNode);
            }
            return dataNode->children().size();
        }
        return 0;
    }
    return m_rootNode ? 1 : 0;
}

int Test::DocumentModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant Test::DocumentModel::data(const QModelIndex &index, int role) const
{
    DocumentModelNode* dataNode = node(index);
    if(dataNode)
    {
        switch(role)
        {
        case Qt::DisplayRole:
            return dataNode->type()->caption(dataNode);

        case Qt::DecorationRole:
            return dataNode->type()->icon(dataNode);

        default:
            break;
        }
    }
    return QVariant();
}

QModelIndex Test::DocumentModel::createIndex(DocumentModelNode* node) const
{
    int row = node->parent() ? node->parent()->children().indexOf(node) : 0;
    return BaseClass::createIndex(row, 0, static_cast<void*>(node));
}
