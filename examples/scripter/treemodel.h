#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit TreeModel(QObject *parent = 0);

signals:

public slots:

};

#endif // TREEMODEL_H
