#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

class QTreeWidget;

class FwMLObject;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    typedef QMainWindow BaseClass;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QTreeWidget* m_treeView;
    FwMLObject* m_rootObject;
};

#endif // MAINWINDOW_H
