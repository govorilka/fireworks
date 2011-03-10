#include "databaseview.h"
#include "database.h"

#include "ui_databaseview.h"

DatabaseView::DatabaseView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DatabaseView),
    m_db(0)
{
    ui->setupUi(this);

    connect(ui->actionAddQuestion, SIGNAL(triggered()), this, SLOT(actionAddQuestion()));
    connect(ui->actionDeleteQuestion, SIGNAL(triggered()), this, SLOT(actionDeleteQuestion()));

}

DatabaseView::~DatabaseView()
{
    delete ui;
}

void DatabaseView::setDatabase(Database* db)
{
    if(m_db != db)
    {
        if(m_db)
        {
            m_db->disconnect(this);
            this->disconnect(m_db);
        }

        m_db = db;
        if(m_db)
        {
            ui->treeView->setModel(m_db);

            QItemSelectionModel* selection = m_db->selectionModel();
            ui->treeView->setSelectionModel(selection);
            connect(selection, SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(currentChanged(QModelIndex)));
            currentChanged(selection->currentIndex());
        }
        else
        {
            ui->treeView->setSelectionModel(0);
            ui->treeView->setModel(0);
        }
    }
}

void DatabaseView::currentChanged(const QModelIndex &current)
{
    if(!current.isValid())
    {
        ui->actionAddQuestion->setEnabled(false);
        ui->actionDeleteQuestion->setEnabled(false);
        ui->actionAddTheme->setEnabled(false);
        ui->actionDeleteTheme->setEnabled(false);
        return;
    }

    DataNode* node = static_cast<DataNode*>(current.internalPointer());
    switch(node->type)
    {
    case DataNode::NT_Root:
        ui->actionAddQuestion->setEnabled(true);
        ui->actionDeleteQuestion->setEnabled(false);
        ui->actionAddTheme->setEnabled(true);
        ui->actionDeleteTheme->setEnabled(false);
        return;

    case DataNode::NT_Question:
        ui->actionAddQuestion->setEnabled(true);
        ui->actionDeleteQuestion->setEnabled(true);
        ui->actionAddTheme->setEnabled(true);
        ui->actionDeleteTheme->setEnabled(false);
        return;

    default:
        Q_ASSERT(false);
        break;
    }
}

DataNode* DatabaseView::currentNode() const
{
    if(m_db)
    {
        QModelIndex index = ui->treeView->selectionModel()->currentIndex();
        if(index.isValid())
        {
            return static_cast<DataNode*>(index.internalPointer());
        }
    }
    return 0;
}

void DatabaseView::actionAddQuestion()
{
    DataNode* node = currentNode();
    if(node)
    {
        m_db->addQuestion(0);
    }
}

void DatabaseView::actionDeleteQuestion()
{
    DataNode* node = currentNode();
    if(node && node->type == DataNode::NT_Question)
    {
        m_db->deleteQuestion(node);
    }
}

void DatabaseView::actionAddTheme()
{

}

void DatabaseView::actionDeleteTheme()
{

}
