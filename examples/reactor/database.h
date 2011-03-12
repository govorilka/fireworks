#ifndef REACTOR_DATABASE_H
#define REACTOR_DATABASE_H

#include <QtCore/qobject.h>

#include <QtGui/qabstractitemview.h>

class FwSQLiteDatabase;

class DataNode
{
public:

    enum Type
    {
        NT_Unknow,
        NT_Root,
        NT_Folder,
        NT_Question,
    };

    DataNode();
    DataNode(DataNode::Type type, int key, const QIcon& icon = QIcon(), const QString& caption = QString());
    ~DataNode();

    inline Type type() const;
    inline int key() const;
    inline DataNode* parent() const;
    inline int row() const;

    inline QList<DataNode*> children() const;

    DataNode* createChild(DataNode::Type type, int key, const QIcon& icon = QIcon(), const QString& caption = QString());
    void deleteChild(DataNode* node);

    inline QString caption() const;
    inline void setCaption(const QString& caption);

    inline QIcon icon() const;
    inline void setIcon(const QIcon& icon);

private:
    int m_row;
    DataNode* m_parent;
    Type m_type;
    int m_key;
    QIcon m_icon;
    QString m_caption;
    QList<DataNode*> m_children;
};

DataNode::Type DataNode::type() const
{
    return m_type;
}

int DataNode::key() const
{
    return m_key;
}

DataNode* DataNode::parent() const
{
    return m_parent;
}

int DataNode::row() const
{
    return m_row;
}

QList<DataNode*> DataNode::children() const
{
    return m_children;
}

QString DataNode::caption() const
{
    return m_caption;
}

void DataNode::setCaption(const QString& caption)
{
    m_caption = caption;
}

QIcon DataNode::icon() const
{
    return m_icon;
}

void DataNode::setIcon(const QIcon& icon)
{
    m_icon = icon;
}

/////////////////////////////////////////////////////////////////////

class Database : public QAbstractItemModel
{
    Q_OBJECT
    typedef QAbstractItemModel BaseClass;

public:
    explicit Database(QObject *parent = 0);
    ~Database();

    inline QItemSelectionModel* selectionModel() const;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    inline QString lastError() const;

    bool open();

    QString questionText(int key) const;
    void setQuestionText(int key, const QString& text);

    void addQuestion(int themeID);
    void deleteQuestion(DataNode* question);

    void addFolder(int key);
    void deleteFolder(DataNode* folder);

    inline QModelIndex createIndex(DataNode* node) const;

    DataNode* currentNode() const;
    void setCurrentNode(DataNode* node);

signals:
    void currentChanged(DataNode* node);

protected:
    mutable QString errorMessage;

    void loadNode(DataNode* parent);
    void loadNodeRoot(DataNode* parent);

    void addNode(DataNode* parent, DataNode::Type type, int key, const QString& caption);
    void deleteNode(DataNode* node);

private:

    FwSQLiteDatabase* m_db;
    DataNode* rootNode;
    QItemSelectionModel* m_selectionModel;
    int m_currentKey;
    DataNode::Type m_currentType;

    QIcon m_folderIcon;
    QIcon m_questionIcon;

private slots:
    void currentChanged(const QModelIndex& current, const QModelIndex& previous);
};

QString Database::lastError() const
{
    return errorMessage;
}

QItemSelectionModel* Database::selectionModel() const
{
    return m_selectionModel;
}

QModelIndex Database::createIndex(DataNode* node) const
{
    return BaseClass::createIndex(node->row(), 0, static_cast<void*>(node));
}

#endif // REACTOR_DATABASE_H
