#include "dataedit.h"

#include "ui_dataeditroot.h"
#include "ui_dataeditquestion.h"

#include "datatype.h"

DataEdit::DataEdit(DataType* type, QWidget *parent) :
    BaseClass(parent),
    m_type(type),
    m_currentNode(0)
{
}

void DataEdit::open(DataNode* node)
{
    if(m_currentNode != node)
    {
        if(m_currentNode)
        {
            close();
        }
        m_currentNode = node;
        loadData(m_currentNode);
    }
}

void DataEdit::save()
{
    if(m_currentNode)
    {
        saveData(m_currentNode);
    }
}

void DataEdit::close()
{
    save();
    m_currentNode = 0;
}

void DataEdit::loadData(DataNode* node)
{
}

void DataEdit::saveData(DataNode* node)
{
}

///////////////////////////////////////////////////////////////////////////

DataEditRoot::DataEditRoot(DataTypeRoot* type, QWidget *parent) :
    BaseClass(type, parent),
    ui(new Ui::DataEditRoot)
{
    ui->setupUi(this);
}

DataEditRoot::~DataEditRoot()
{
    delete ui;
}

///////////////////////////////////////////////////////////////////////////

DataEditQuestion::DataEditQuestion(DataTypeQuestion* type, QWidget *parent) :
    BaseClass(type, parent),
    ui(new Ui::DataEditQuestion)
{
    ui->setupUi(this);
}

DataEditQuestion::~DataEditQuestion()
{
    delete ui;
}
