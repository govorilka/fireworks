#include <QtGui/qtreewidget.h>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    BaseClass(parent),
    m_treeView(0)
{
    m_treeView = new QTreeWidget(this);

    QTreeWidgetItem* itemScene = new QTreeWidgetItem(m_treeView);
    itemScene->setText(0, "Scene");

    QTreeWidgetItem* itemSize = new QTreeWidgetItem(itemScene);
    itemSize->setText(0, "size");

    QTreeWidgetItem* itemBackground = new QTreeWidgetItem(itemScene);
    itemBackground->setText(0, "background");

    setCentralWidget(m_treeView);
}

MainWindow::~MainWindow()
{
}
