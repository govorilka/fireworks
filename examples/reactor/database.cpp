#include <QtCore/qfile.h>
#include <QtCore/qdebug.h>

#include "database.h"

#include "fwdb/fwsqlite.h"

DataNode::DataNode() :
    m_row(0),
    m_parent(0),
    m_type(NT_Unknow),
    m_key(0)
{
}

DataNode::DataNode(DataNode::Type type, int key, const QIcon& icon, const QString& caption) :
    m_row(0),
    m_parent(0),
    m_type(type),
    m_key(key),
    m_icon(icon),
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

DataNode* DataNode::createChild(DataNode::Type type, int key, const QIcon& icon, const QString& caption)
{
    DataNode* newChild = new DataNode(type, key, icon, nextChildCaption(caption));
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

/////////////////////////////////////////////////////////////////////

Database::Database(QObject *parent) :
    BaseClass(parent),
    m_db(new FwSQLiteDatabase(this)),
    rootNode(new DataNode(DataNode::NT_Root, 0)),
    m_selectionModel(new QItemSelectionModel(this)),
    m_currentKey(0),
    m_currentType(DataNode::NT_Unknow)
{
    m_folderIcon.addPixmap(QPixmap(":/reactor/images/datatree/folder.png"));
    m_folderIcon.addPixmap(QPixmap(":/reactor/images/dataedit/folder.png"));
    m_questionIcon.addPixmap(QPixmap(":/reactor/images/datatree/question.png"));
    m_questionIcon.addPixmap(QPixmap(":/reactor/images/dataedit/question.png"));

    QIcon databaseIcon;
    databaseIcon.addPixmap(QPixmap(":/reactor/images/datatree/database.png"));
    databaseIcon.addPixmap(QPixmap(":/reactor/images/dataedit/database.png"));
    rootNode->setCaption(tr("Reactor"));
    rootNode->setIcon(databaseIcon);

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
        return createIndex(rootNode);
    }

    DataNode* parentNode = static_cast<DataNode*>(parent.internalPointer());
    foreach(DataNode* child, parentNode->children())
    {
        if(child->row() == row && 0 == column)
        {
            return createIndex(child);
        }
    }

    return QModelIndex();
}

QModelIndex Database::parent(const QModelIndex &child) const
{
    if(child.isValid())
    {
        DataNode* parentNode = static_cast<DataNode*>(child.internalPointer())->parent();
        if(parentNode)
        {
            return createIndex(parentNode);
        }
    }
    return QModelIndex();
}

int Database::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        DataNode* parentNode = static_cast<DataNode*>(parent.internalPointer());
        if(parentNode->children().isEmpty())
        {
            const_cast<Database*>(this)->loadNode(parentNode);
        }
        return parentNode->children().size();
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
            return node->caption();

        case Qt::DecorationRole:
            return node->icon();

        default:
            break;
        }
    }
    return QVariant();
}

void Database::loadNode(DataNode* parent)
{
    switch(parent->type())
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
        FwSQLiteQuery folderQuery = m_db->query("SELECT folderid,caption FROM folder");
        while(folderQuery.step())
        {
            rootNode->createChild(DataNode::NT_Folder,
                                  folderQuery.columnInt(0),
                                  m_folderIcon,
                                  folderQuery.columnText(1));
        }

        FwSQLiteQuery questionQuery = m_db->query("SELECT questionid FROM question");
        while(questionQuery.step())
        {
            rootNode->createChild(DataNode::NT_Question,
                                  questionQuery.columnInt(0),
                                  m_questionIcon,
                                  tr("Question %1"));
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
        if(node->type() != m_currentType || node->key() != m_currentKey)
        {
            m_currentType = node->type();
            m_currentKey = node->key();
            currentChanged(node);
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

void Database::addQuestion(int themeID)
{
    try
    {
        FwSQLiteQuery questionQuery = m_db->query("INSERT INTO question (htmltext) VALUES (?1)");
        questionQuery.bindText(1, tr("New question"));
        questionQuery.step();
        addNode(rootNode, DataNode::NT_Question, m_db->lastInsertKey(), tr("Question %1"));
    }
    catch(FwSQLiteException& e)
    {
        errorMessage = QString("SQLite: %1").arg(e.error);
        qDebug() << errorMessage;
    }
}

void Database::deleteQuestion(DataNode* question)
{
    try
    {
        FwSQLiteQuery questionQuery = m_db->query("DELETE FROM question WHERE questionid=?1");
        questionQuery.bindInt(1, question->key());
        questionQuery.step();        
        deleteNode(question);
    }
    catch(FwSQLiteException& e)
    {
        errorMessage = QString("SQLite: %1").arg(e.error);
        qDebug() << errorMessage;
    }
}

DataNode* Database::currentNode() const
{
    QModelIndex index = m_selectionModel->currentIndex();
    if(index.isValid())
    {
        return static_cast<DataNode*>(index.internalPointer());
    }
    return 0;
}

void Database::setCurrentNode(DataNode* node)
{
    QModelIndex index;
    if(node)
    {
        index = createIndex(node);
    }
    m_selectionModel->setCurrentIndex(index, QItemSelectionModel::Clear | QItemSelectionModel::SelectCurrent);
}

void Database::addFolder(int key)
{
    try
    {
        QString caption = rootNode->nextChildCaption(tr("New folder %1"));
        FwSQLiteQuery questionQuery = m_db->query("INSERT INTO folder(caption) VALUES(?1)");
        questionQuery.bindText(1, caption);
        questionQuery.step();
        addNode(rootNode, DataNode::NT_Folder, m_db->lastInsertKey(), caption);
    }
    catch(FwSQLiteException& e)
    {
        errorMessage = QString("SQLite: %1").arg(e.error);
        qDebug() << errorMessage;
    }
}
void Database::deleteFolder(DataNode* folder)
{
    try
    {
        FwSQLiteQuery folderQuery = m_db->query("DELETE FROM folder WHERE folderid=?1");
        folderQuery.bindInt(1, folder->key());
        folderQuery.step();
        deleteNode(folder);
    }
    catch(FwSQLiteException& e)
    {
        errorMessage = QString("SQLite: %1").arg(e.error);
        qDebug() << errorMessage;
    }
}

void Database::addNode(DataNode* parent, DataNode::Type type, int key, const QString& caption)
{
    QModelIndex parentIndex = createIndex(parent);
    beginInsertRows(parentIndex, parent->children().size(), parent->children().size());
    DataNode* newChild = parent->createChild(type, key, type == DataNode::NT_Question ? m_questionIcon : m_folderIcon, caption);
    endInsertRows();
    setCurrentNode(newChild);
}

void Database::deleteNode(DataNode* node)
{
    if(node && node->parent())
    {
        beginRemoveRows(createIndex(node->parent()), node->row(), node->row());
        rootNode->deleteChild(node);
        endRemoveRows();
    }
}

QString Database::tickets(int variantCount, int questionCount) const
{
    try
    {
        QList<int> questionsIDs;
        QHash<int, QString> questions;
        FwSQLiteQuery questionQuery = m_db->query("SELECT questionid, htmltext FROM question");
        while(questionQuery.step())
        {
            questionsIDs.append(questionQuery.columnInt(0));
            questions.insert(questionQuery.columnInt(0), questionQuery.columnText(1));
        }

        qsrand(QDateTime::currentDateTime().toTime_t());

        QString text;
        QList<int> rands;
        for(int i = 0; i < variantCount; i++)
        {
            QString strQuestions;
            for(int j = 0; j < questionCount; j++)
            {
                int rand = qrand() % questions.size();
                while(rands.contains(rand))
                {
                    rand = qrand() % questions.size();
                }

                if(rands.size() == questionsIDs.size() - 1)
                {
                    rands.clear();
                }
                else
                {
                    rands.append(rand);
                }

                qDebug() << questions.size() << rand << rands;

                int questionID = questionsIDs.at(rand);
                strQuestions.append(QString("<h3>Question %1</h3>%2").arg(j + 1).arg(questions.value(questionID)));
            }
            text.append(QString("<h2>Ticket #%1</h2>%2").arg(i + 1).arg(strQuestions));
        }
        return text;
    }
    catch(FwSQLiteException& e)
    {
        errorMessage = QString("SQLite: %1").arg(e.error);
        qDebug() << errorMessage;
    }

    return QString();
}
