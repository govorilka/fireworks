#include <QtGui/qmessagebox.h>
#include <QtGui/qapplication.h>
#include <QtGui/qdockwidget.h>
#include <QtGui/qtreeview.h>

#include <stdio.h>
#include <stdlib.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "database.h"
#include "databaseview.h"

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

    connect(m_db, SIGNAL(currentChanged(DataNode*)), this, SLOT(currentChanged(DataNode*)));
    currentChanged(m_db->currentNode());

    QDockWidget* dataTreeDock = new QDockWidget(tr("Database"), this);
    addDockWidget(Qt::LeftDockWidgetArea, dataTreeDock);

    DatabaseView* databaseView = new DatabaseView(dataTreeDock);
    dataTreeDock->setWidget(databaseView);
    databaseView->setDatabase(m_db);

    connect(ui->actionBold, SIGNAL(triggered()), this,  SLOT(textEditBold()));
    connect(ui->actionItalic, SIGNAL(triggered()), this,  SLOT(textEditItanic()));
    connect(ui->actionUnderline, SIGNAL(triggered()), this,  SLOT(textEditUnderline()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::currentChanged(DataNode* node)
{
    if(m_questionId)
    {
        m_db->setQuestionText(m_questionId, ui->textEdit->toHtml());
        ui->textEdit->setHtml("");
        m_questionId = 0;
    }

    if(node)
    {
        ui->stackedWidget->setCurrentIndex(node->type() - 1);
        ui->nodeIcon->setPixmap(node->icon().pixmap(32));
        ui->nodeTitle->setText(node->caption());

        if(node->type() == DataNode::NT_Question)
        {
            m_questionId = node->key();
            ui->textEdit->setHtml(m_db->questionText(m_questionId));
        }

        ui->stackedWidget->show();
        ui->nodeIcon->show();
        ui->nodeTitle->show();
    }
    else
    {
        ui->stackedWidget->hide();
        ui->nodeIcon->hide();
        ui->nodeTitle->hide();
    }
}

void MainWindow::textEditBold()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(QFont::Bold);
    //fmt.setFontWeight(actionTextBold->isChecked() ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::textEditItanic()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(true);
    mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::textEditUnderline()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(true);
    mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::mergeFormatOnWordOrSelection(const QTextCharFormat& fmt)
{
    QTextCursor cursor = ui->textEdit->textCursor();
    if (!cursor.hasSelection())
    {
        cursor.select(QTextCursor::WordUnderCursor);
    }
    cursor.mergeCharFormat(fmt);
    ui->textEdit->mergeCurrentCharFormat(fmt);
}
