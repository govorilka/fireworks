#include <QtGui/qmessagebox.h>
#include <QtGui/qapplication.h>
#include <QtGui/qdockwidget.h>
#include <QtGui/qtreeview.h>

#include <stdio.h>
#include <stdlib.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "database.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_db(new Database(this)),
    m_questionId(0)
{
    ui->setupUi(this);

    if(!m_db->open())
    {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Cannot open database: %1").arg(m_db->lastError()));
        ::exit(-1);
    }

    connect(m_db, SIGNAL(currentChanged(int,int)), this, SLOT(currentChanged(int,int)));

    QDockWidget* dataTreeDock = new QDockWidget(tr("Database"), this);
    addDockWidget(Qt::LeftDockWidgetArea, dataTreeDock);

    QTreeView* dataTree = new QTreeView(dataTreeDock);
    dataTreeDock->setWidget(dataTree);
    dataTree->setModel(m_db);
    dataTree->setHeaderHidden(true);
    dataTree->setSelectionModel(m_db->selectionModel());
    dataTree->setAlternatingRowColors(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::currentChanged(int type, int key)
{
    if(m_questionId)
    {
        m_db->setQuestionText(m_questionId, ui->centralWidget->toHtml());
        ui->centralWidget->setHtml("");
        m_questionId = 0;
    }

    if(type == DataNode::NT_Question)
    {
        m_questionId = key;
        ui->centralWidget->setHtml(m_db->questionText(m_questionId));
    }
}
