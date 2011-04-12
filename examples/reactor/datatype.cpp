#include <QtCore/qdebug.h>

#include "fwdb/fwsqlite.h"

#include "datatype.h"
#include "dataedit.h"
#include "datanode.h"
#include "database.h"

DataType::DataType(Database* db, const QString& className) :
    m_db(db),
    m_className(className),
    m_icon(QString(":/reactor/images/datatree/%1.png").arg(className.toLower())),
    m_edit(0)
{
    m_icon.addPixmap(QString(":/reactor/images/dataedit/%1.png").arg(className.toLower()));
}

DataType::~DataType()
{
}

bool DataType::hasChildren() const
{
    return false;
}

QVector<DataType*> DataType::childrenTypes() const
{
    return QVector<DataType*>();
}

void DataType::loadChildren(DataNode* node)
{
    Q_UNUSED(node);
}

DataNode* DataType::addNode(DataNode* parent)
{
    int key = 0;
    QString caption;
    if(dataInsert(parent, key, caption))
    {
        QModelIndex parentIndex = m_db->createIndex(parent);

        m_db->beginInsertRows(parentIndex, parent->children().size(), parent->children().size());
        DataNode* newChild = parent->createChild(this, key, caption);
        m_db->endInsertRows();

        m_db->setCurrentNode(newChild);

        return newChild;
    }
    return 0;
}

void DataType::removeNode(DataNode* node)
{

}

///////////////////////////////////////////////////////////////////////////////

DataTypeRoot::DataTypeRoot(Database* db) :
   BaseClass(db, "Database")
{
}

DataEdit* DataTypeRoot::createEdit(QWidget* parent)
{
    return new DataEditRoot(this, parent);
}

bool DataTypeRoot::hasChildren() const
{
    return true;
}

QVector<DataType*> DataTypeRoot::childrenTypes() const
{
    return QVector<DataType*>() << m_typeTheme << m_typeQuestion;
}

void DataTypeRoot::loadChildren(DataNode* node)
{
    try
    {
        FwSQLiteDatabase* sqlite = db()->sqlite();
        FwSQLiteQuery folderQuery = sqlite->query("SELECT folderid,caption FROM folder");
        while(folderQuery.step())
        {
            node->createChild(m_typeTheme,
                              folderQuery.columnInt(0),
                              folderQuery.columnText(1));
        }

        FwSQLiteQuery questionQuery = sqlite->query("SELECT questionid FROM question");
        while(questionQuery.step())
        {
            node->createChild(m_typeQuestion,
                              questionQuery.columnInt(0),
                              Database::tr("Question %1"));
        }
    }
    catch(FwSQLiteException& e)
    {
       qDebug() << QString("SQLite: %1").arg(e.error);
    }
}

bool DataTypeRoot::dataInsert(DataNode* parent, int& key, QString& caption)
{
    Q_UNUSED(parent);
    Q_UNUSED(caption);
    Q_UNUSED(key);
    return false;
}

///////////////////////////////////////////////////////////////////////////////

DataTypeTheme::DataTypeTheme(Database* db) :
   BaseClass(db, "Theme")
{
}

DataEdit* DataTypeTheme::createEdit(QWidget* parent)
{
    return new DataEdit(this, parent);
}

bool DataTypeTheme::dataInsert(DataNode* parent, int& key, QString& caption)
{
    Q_UNUSED(parent);
    Q_UNUSED(caption);
    Q_UNUSED(key);
    return false;
}

///////////////////////////////////////////////////////////////////////////////

DataTypeQuestion::DataTypeQuestion(Database* db) :
   BaseClass(db, "Question")
{
}

DataEdit* DataTypeQuestion::createEdit(QWidget* parent)
{
    return new DataEditQuestion(this, parent);
}

bool DataTypeQuestion::dataInsert(DataNode* parent, int& key, QString& caption)
{
    try
    {
        caption = Database::tr("Question %1");
        FwSQLiteDatabase* sqlite = db()->sqlite();
        FwSQLiteQuery questionQuery = sqlite->query("INSERT INTO question (htmltext) VALUES (?1)");
        questionQuery.bindText(1, caption);
        questionQuery.step();
        key = sqlite->lastInsertKey();
        return true;
    }
    catch(FwSQLiteException& e)
    {
        qDebug() << QString("SQLite: %1").arg(e.error);
    }
    return false;
}
