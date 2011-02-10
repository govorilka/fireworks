#ifndef SCRIPTER_TREEMODEL_H
#define SCRIPTER_TREEMODEL_H

#include <QtGui/qabstractitemview.h>

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
    typedef QAbstractItemModel BaseClass;

public:
    explicit TreeModel(QObject *parent = 0);
};

#endif // TREEMODEL_H
