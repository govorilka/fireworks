#include "mainwindow.h"
#include "treemodel.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_model(new TreeModel(this))
{
    ui->setupUi(this);
    ui->centralWidget->setModel(m_model);
}

MainWindow::~MainWindow()
{
    delete ui;
}
