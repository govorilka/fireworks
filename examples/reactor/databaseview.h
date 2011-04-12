#ifndef DATABASEVIEW_H
#define DATABASEVIEW_H

#include <QWidget>

#include <QtGui/qabstractitemview.h>

namespace Ui
{
    class DatabaseView;
}

class Database;
class DataNode;

class DatabaseView : public QWidget
{
    Q_OBJECT

public:
    explicit DatabaseView(QWidget *parent = 0);
    ~DatabaseView();

    inline Database* database() const;
    void setDatabase(Database* db);

    DataNode* currentNode() const;

private:
    Ui::DatabaseView *ui;
    Database* m_db;

private slots:
    void currentChanged(const QModelIndex & current);

    void actionAdd();
    void actionDelete();
};

Database* DatabaseView::database() const
{
    return m_db;
}

#endif // DATABASEVIEW_H
