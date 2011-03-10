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
        NT_Question,
    };

    DataNode();
    ~DataNode();

    int row;
    int column;

    DataNode* parent;
    QList<DataNode*> children;

    QString caption;
    QIcon icon;
    Type type;
    int key;
};

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

    inline QModelIndex createIndex(DataNode* node) const;

signals:
    void currentChanged(int type, int key);

protected:
    mutable QString errorMessage;

    void loadNode(DataNode* parent);
    void loadNodeRoot(DataNode* parent);

private:

    FwSQLiteDatabase* m_db;
    DataNode* rootNode;
    QItemSelectionModel* m_selectionModel;
    int m_currentKey;
    DataNode::Type m_currentType;

    QPixmap m_dbPixmap;
    QPixmap m_questionPixmap;

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
    return BaseClass::createIndex(node->row, node->column, static_cast<void*>(node));
}

#endif // REACTOR_DATABASE_H
