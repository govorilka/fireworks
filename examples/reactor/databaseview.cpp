#include "databaseview.h"

#include "database.h"
#include "datanode.h"
#include "datatype.h"

#include "selectdatatypedialog.h"

#include "ui_databaseview.h"

DatabaseView::DatabaseView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DatabaseView),
    m_db(0)
{
    ui->setupUi(this);

    connect(ui->actionAdd, SIGNAL(triggered()), this, SLOT(actionAdd()));
    connect(ui->actionDelete, SIGNAL(triggered()), this, SLOT(actionDelete()));
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
    /*if(!current.isValid())
    {
        ui->actionAddQuestion->setEnabled(false);
        ui->actionDeleteQuestion->setEnabled(false);
        ui->actionAddFolder->setEnabled(false);
        ui->actionDeleteFolder->setEnabled(false);
        return;
    }

    DataNode* node = static_cast<DataNode*>(current.internalPointer());
    switch(node->type())
    {
    case DataNode::NT_Root:
        ui->actionAddQuestion->setEnabled(true);
        ui->actionDeleteQuestion->setEnabled(false);
        ui->actionAddFolder->setEnabled(true);
        ui->actionDeleteFolder->setEnabled(false);
        return;

    case DataNode::NT_Question:
        ui->actionAddQuestion->setEnabled(true);
        ui->actionDeleteQuestion->setEnabled(true);
        ui->actionAddFolder->setEnabled(true);
        ui->actionDeleteFolder->setEnabled(false);
        return;

    case DataNode::NT_Folder:
        ui->actionAddQuestion->setEnabled(true);
        ui->actionDeleteQuestion->setEnabled(false);
        ui->actionAddFolder->setEnabled(true);
        ui->actionDeleteFolder->setEnabled(true);
        return;

    default:
        Q_ASSERT(false);
        break;
    }*/
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

void DatabaseView::actionAdd()
{
    DataNode* node = currentNode();
    if(node)
    {
        DataType* type = node->type();
        if(type->hasChildren())
        {
            QVector<DataType*> childrenTypes = type->childrenTypes();
            if(!childrenTypes.isEmpty())
            {
                DataType* newType = childrenTypes.at(0);
                if(childrenTypes.size() > 1)
                {
                    if(!(newType = SelectDataTypeDialog::getType(this, childrenTypes, newType)))
                    {
                        return;
                    }
                }
                newType->addNode(node);
            }
        }
    }
}

void DatabaseView::actionDelete()
{
    DataNode* node = currentNode();
    if(node)
    {
        //m_db->deleteQuestion(node);
    }
}
