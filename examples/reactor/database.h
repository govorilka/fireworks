#ifndef REACTOR_DATABASE_H
#define REACTOR_DATABASE_H

#include <QtCore/qobject.h>
#include <QtCore/qvector.h>

#include <QtGui/qabstractitemview.h>

class FwSQLiteDatabase;
class DataType;
class DataNode;

class Database : public QAbstractItemModel
{
    Q_OBJECT
    typedef QAbstractItemModel BaseClass;

public:
    friend class DataType;

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

    QModelIndex createIndex(DataNode* node) const;

    DataNode* currentNode() const;
    void setCurrentNode(DataNode* node);

    QString tickets(int variantCount, int questionCount) const;

    inline QVector<DataType*> types() const;

    inline FwSQLiteDatabase* sqlite() const;

signals:
    void currentChanged(DataNode* current);

protected:
    mutable QString errorMessage;

    void deleteNode(DataNode* node);

private:

    FwSQLiteDatabase* m_db;

    DataNode* rootNode;
    QVector<DataType*> m_types;

    QItemSelectionModel* m_selectionModel;
    DataNode* m_currentNode;

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

QVector<DataType*> Database::types() const
{
    return m_types;
}

FwSQLiteDatabase* Database::sqlite() const
{
    return m_db;
}

#endif // REACTOR_DATABASE_H
