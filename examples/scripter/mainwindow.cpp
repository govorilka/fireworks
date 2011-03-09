#include <QtGui/qfiledialog.h>

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
    connect(ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(open()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open fwml file"));
    open(fileName);
}

void MainWindow::open(const QString& fileName)
{
}
