#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

class QTreeWidget;
class QTreeWidgetItem;

class FwMLObject;
class FwMLNode;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    typedef QMainWindow BaseClass;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void addNode(QTreeWidgetItem* parent, FwMLNode* node);

private:
    QTreeWidget* m_treeView;
    FwMLObject* m_rootObject;
};

#endif // MAINWINDOW_H
