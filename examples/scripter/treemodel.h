#ifndef SCRIPTER_TREEMODEL_H
#define SCRIPTER_TREEMODEL_H

#include <QtGui/qabstractitemview.h>

class FwMLObject;
class FwMLNode;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
    typedef QAbstractItemModel BaseClass;

public:
    explicit TreeModel(QObject *parent = 0);
    ~TreeModel();

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void parseFile(const QString& fileName);

protected:
    int nodeRow(FwMLNode* node) const;

private:
    FwMLObject* m_rootObject;
};

#endif // SCRIPTER_TREEMODEL_H
