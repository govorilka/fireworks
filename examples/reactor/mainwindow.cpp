#include <QtCore/qdebug.h>

#include <QtGui/qmessagebox.h>
#include <QtGui/qapplication.h>
#include <QtGui/qdockwidget.h>
#include <QtGui/qtreeview.h>
#include <QtGui/qprinter.h>
#include <QtGui/qprintdialog.h>
#include <QtGui/qprintpreviewdialog.h>
#include <QtGui/qtextdocument.h>
#include <QtGui/qfiledialog.h>
#include <QtGui/qdesktopservices.h>

#include <stdio.h>
#include <stdlib.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "database.h"
#include "databaseview.h"
#include "datanode.h"
#include "datatype.h"
#include "dataedit.h"

#include "testdocumentmodel.h"
#include "testdocumentview.h"

MainWindow::MainWindow(QWidget *parent) :
    BaseClass(parent),
    ui(new Ui::MainWindow),
    m_db(new Database(this)),
    m_printer(new QPrinter()),
    m_currentEdit(0),
    m_testDocument(0)
{
    m_testDocument = new Test::DocumentModel(this);

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

    ui->menuToolbars->addAction(ui->toolsReports->toggleViewAction());

    QDockWidget* dataTreeDock = new QDockWidget(tr("Database"), this);
    addDockWidget(Qt::LeftDockWidgetArea, dataTreeDock);
    ui->menuDockPanels->addAction(dataTreeDock->toggleViewAction());

    Test::DocumentView* documentView = new Test::DocumentView(m_testDocument, dataTreeDock);
    dataTreeDock->setWidget(documentView);

    //connect(ui->actionBold, SIGNAL(triggered()), this,  SLOT(textEditBold()));
    //connect(ui->actionItalic, SIGNAL(triggered()), this,  SLOT(textEditItanic()));
    //connect(ui->actionUnderline, SIGNAL(triggered()), this,  SLOT(textEditUnderline()));
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(documentNew()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionPrint, SIGNAL(triggered()), this, SLOT(print()));

    QVector<DataType*> types = m_db->types();
    foreach(DataType* type, types)
    {
        DataEdit* edit = type->createEdit(this);
        edit->setObjectName("edit" + type->className());
        type->setEdit(edit);
        ui->stackedWidget->addWidget(edit);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_printer;
}

void MainWindow::currentChanged(DataNode* node)
{
    if(m_currentEdit)
    {
        m_currentEdit->close();
        m_currentEdit = 0;
    }

    if(node)
    {
        DataType* type = node->type();

        DataEdit* edit = type->edit();
        if(edit)
        {
            m_currentEdit = edit;
            m_currentEdit->open(node);
            ui->stackedWidget->setCurrentWidget(m_currentEdit);
            ui->stackedWidget->show();
        }
        else
        {
            ui->stackedWidget->hide();
        }


        ui->nodeIcon->setPixmap(type->icon().pixmap(32));
        ui->nodeIcon->show();

        ui->nodeTitle->setText(node->caption());
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
    /*QTextCursor cursor = ui->textEdit->textCursor();
    if (!cursor.hasSelection())
    {
        cursor.select(QTextCursor::WordUnderCursor);
    }
    cursor.mergeCharFormat(fmt);
    ui->textEdit->mergeCurrentCharFormat(fmt);*/
}

void MainWindow::print()
{
    QPrintDialog* printDialog = new QPrintDialog(m_printer, this);
    if(printDialog->exec() == QDialog::Accepted)
    {
        QTextDocument textDocument;
        textDocument.setHtml(QString("<html><body>%1</body><html>").arg(m_db->tickets(10, 3)));
        textDocument.print(m_printer);
    }
}

void MainWindow::documentNew()
{
    QString documentPath = QFileDialog::getSaveFileName(this,
                                                        tr("Create new test"),
                                                        QDesktopServices::displayName(QDesktopServices::DocumentsLocation),
                                                        "*.test");
    if(!documentPath.isEmpty())
    {
        if(QFileInfo(documentPath).suffix().isEmpty())
        {
            documentPath += ".test"; //TODO: hard conding :)
        }
        m_testDocument->open(documentPath);
    }
}
