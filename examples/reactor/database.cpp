#include <QtCore/qfile.h>
#include <QtCore/qdebug.h>

#include "database.h"
#include "datatype.h"
#include "datanode.h"

#include "fwdb/fwsqlite.h"

Database::Database(QObject *parent) :
    BaseClass(parent),
    m_db(new FwSQLiteDatabase(this)),
    m_selectionModel(new QItemSelectionModel(this)),
    m_currentNode(0)
{
    DataTypeRoot* typeRoot = new DataTypeRoot(this);
    m_types.append(typeRoot);

    DataTypeTheme* typeTheme = new DataTypeTheme(this);
    m_types.append(typeTheme);
    typeRoot->setDataTypeTheme(typeTheme);

    DataTypeQuestion* typeQuestion = new DataTypeQuestion(this);
    m_types.append(typeQuestion);
    typeRoot->setDataTypeQuestion(typeQuestion);

    rootNode = new DataNode(typeRoot, 0, tr("Reactor"));

    connect(m_selectionModel, SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(currentChanged(QModelIndex,QModelIndex)));
}

Database::~Database()
{
    delete rootNode;

    foreach(DataType* type, m_types)
    {
        delete type;
    }
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
        DataType* parentType = parentNode->type();
        if(parentType->hasChildren() && parentNode->children().isEmpty())
        {
            parentType->loadChildren(parentNode);
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
            return node->type()->icon();

        default:
            break;
        }
    }
    return QVariant();
}

void Database::currentChanged(const QModelIndex& current, const QModelIndex& previous)
{    
    Q_UNUSED(previous);

    DataNode* currentNode = 0;
    if(current.isValid())
    {
        currentNode = static_cast<DataNode*>(current.internalPointer());
    }

    if(m_currentNode != currentNode)
    {
        m_currentNode = currentNode;
        emit currentChanged(m_currentNode);
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
    /*try
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
    }*/
}

void Database::deleteQuestion(DataNode* question)
{
    /*try
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
    }*/
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
    /*try
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
    }*/
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

QModelIndex Database::createIndex(DataNode* node) const
{
    return BaseClass::createIndex(node->row(), 0, static_cast<void*>(node));
}
