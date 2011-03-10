#include <QtCore/qfile.h>
#include <QtCore/qdebug.h>

#include "database.h"

#include "fwdb/fwsqlite.h"

DataNode::DataNode() :
    row(0),
    column(0),
    parent(0),
    type(NT_Unknow),
    key(0)
{
}

DataNode::~DataNode()
{
    foreach(DataNode* node, children)
    {
        delete node;
    }
}

/////////////////////////////////////////////////////////////////////

Database::Database(QObject *parent) :
    BaseClass(parent),
    m_db(new FwSQLiteDatabase(this)),
    rootNode(new DataNode()),
    m_selectionModel(new QItemSelectionModel(this)),
    m_currentKey(0),
    m_currentType(DataNode::NT_Unknow)
{
    m_dbPixmap.load(":/reactor/images/datatree/database.png");
    m_questionPixmap.load(":/reactor/images/datatree/question.png");
    rootNode->caption = tr("Reactor");
    rootNode->type = DataNode::NT_Root;
    rootNode->icon = QIcon(m_dbPixmap);

    connect(m_selectionModel, SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(currentChanged(QModelIndex,QModelIndex)));
}

Database::~Database()
{
    delete rootNode;
}

bool Database::open()
{
    try
    {
        m_db->open("reactor.db");
        m_db->execFile(":/reactor/sql/init.sql");
    }
    catch(FwSQLiteException& e)
    {
        m_db->rollback();
        errorMessage = QString("SQLite: %1").arg(e.error);
        return false;
    }

    return true;
}

QModelIndex Database::index(int row, int column, const QModelIndex &parent) const
{
    if(!parent.isValid())
    {
        return createIndex(0, 0, static_cast<void*>(rootNode));
    }

    DataNode* parentNode = static_cast<DataNode*>(parent.internalPointer());
    foreach(DataNode* child, parentNode->children)
    {
        if(child->row == row && child->column == column)
        {
            return createIndex(row, column, static_cast<void*>(child));
        }
    }

    return QModelIndex();
}

QModelIndex Database::parent(const QModelIndex &child) const
{
    if(child.isValid())
    {
        DataNode* parentNode = static_cast<DataNode*>(child.internalPointer())->parent;
        if(parentNode)
        {
            return createIndex(parentNode->row, parentNode->column, static_cast<void*>(parentNode));
        }
    }
    return QModelIndex();
}

int Database::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        DataNode* parentNode = static_cast<DataNode*>(parent.internalPointer());
        if(parentNode->children.isEmpty())
        {
            const_cast<Database*>(this)->loadNode(parentNode);
        }
        return parentNode->children.size();
    }
    return 1;
}

int Database::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant Database::data(const QModelIndex &index, int role) const
{
    if(index.isValid())
    {
        DataNode* node = static_cast<DataNode*>(index.internalPointer());
        switch(role)
        {
        case Qt::DisplayRole:
            return node->caption;

        case Qt::DecorationRole:
            return node->icon;

        default:
            break;
        }
    }
    return QVariant();
}

void Database::loadNode(DataNode* parent)
{
    switch(parent->type)
    {
    case DataNode::NT_Root:
        loadNodeRoot(parent);

    default:
        break;
    }
}

void Database::loadNodeRoot(DataNode* parent)
{
    try
    {
        int index = 0;
        FwSQLiteQuery questionQuery = m_db->query("SELECT questionid FROM question");
        while(questionQuery.step())
        {
            DataNode* child = new DataNode();
            child->parent = parent;
            parent->children.append(child);

            child->row = index;
            child->type = DataNode::NT_Question;
            child->key = questionQuery.columnInt(0);
            child->caption = tr("Question %1").arg(++index);
            child->icon = QIcon(m_questionPixmap);
        }
    }
    catch(FwSQLiteException& e)
    {
        errorMessage = QString("SQLite: %1").arg(e.error);
    }
}

void Database::currentChanged(const QModelIndex& current, const QModelIndex& previous)
{
    Q_UNUSED(previous);

    if(current.isValid())
    {
        DataNode* node = static_cast<DataNode*>(current.internalPointer());
        if(node->type != m_currentType || node->key != m_currentKey)
        {
            m_currentType = node->type;
            m_currentKey = node->key;
            currentChanged(m_currentType, m_currentKey);
        }
    }

}

QString Database::questionText(int key) const
{
    try
    {
        FwSQLiteQuery questionQuery = m_db->query("SELECT htmltext FROM question WHERE questionid = ?1");
        questionQuery.bindInt(1, key);
        if(questionQuery.step())
        {
            return questionQuery.columnText(0);
        }
    }
    catch(FwSQLiteException& e)
    {
        errorMessage = QString("SQLite: %1").arg(e.error);
        qDebug() << errorMessage;
    }

    return "";
}

void Database::setQuestionText(int key, const QString& text)
{
    try
    {
        FwSQLiteQuery questionQuery = m_db->query("UPDATE question SET htmltext = ?1 WHERE questionid = ?2");
        questionQuery.bindText(1, text);
        questionQuery.bindInt(2, key);
        questionQuery.step();
    }
    catch(FwSQLiteException& e)
    {
        errorMessage = QString("SQLite: %1").arg(e.error);
        qDebug() << errorMessage;
    }
}
