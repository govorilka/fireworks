#include <QtGui/qtreewidget.h>

#include "mainwindow.h"

#include "fwcore/fwml.h"

MainWindow::MainWindow(QWidget *parent) :
    BaseClass(parent),
    m_treeView(0),
    m_rootObject(0)
{
    m_treeView = new QTreeWidget(this);
    m_treeView->setColumnCount(2);
    m_treeView->setHeaderLabels(QStringList() << "Node" << "Value");

    setCentralWidget(m_treeView);

    QByteArray fwml = "\"Scene\" : {\n"
                      "\"background\" : \"123.png\", \n"
                      "\"size\" : { \n"
                      "\"width\" : \"100\", \n"
                      "\"height\" : \"100\" }, \n"
                      "array[1, 2, 3, 5] \n"
                      "}";

    QTreeWidgetItem* rootItem = new QTreeWidgetItem(m_treeView);
    rootItem->setText(0, "root");

    m_rootObject = new FwMLObject();

    QString error;
    if(m_rootObject->parse(fwml, &error))
    {
        addNode(rootItem, m_rootObject);
    }
    else
    {
        QTreeWidgetItem* errorItem = new QTreeWidgetItem(rootItem);
        errorItem->setText(0, "Error");
        errorItem->setText(1, error);
    }
}

MainWindow::~MainWindow()
{
    delete m_rootObject;
}

void MainWindow::addNode(QTreeWidgetItem* parent, FwMLNode* node)
{
    switch(node->type())
    {
    case FwMLNode::T_Object:
        {
            FwMLObject* object = node->toObject();
            QHash<QByteArray, FwMLNode*> attributes = object->attributes();
            for(QHash<QByteArray, FwMLNode*>::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter)
            {
                QTreeWidgetItem* childItem = new QTreeWidgetItem(parent);
                childItem->setText(0, QString::fromUtf8(iter.key()));
                addNode(childItem, iter.value());
            }
        }
        break;

    case FwMLNode::T_String:
        {
            FwMLString* string = node->toString();
            parent->setText(1, QString::fromUtf8(string->value));
        }
        break;

    case FwMLNode::T_Number:
        {
            FwMLNumber* number = node->toNumber();
            parent->setText(1, QString::fromUtf8(number->toUtf8()));
        }
        break;

    case FwMLNode::T_Array:
        {
            FwMLArray* array = node->toArray();
            int i = 0;
            foreach(FwMLNode* child, array->data)
            {
                QTreeWidgetItem* childItem = new QTreeWidgetItem(parent);
                childItem->setText(0, QString::number(i++));
                addNode(childItem, child);
            }
        }
        break;

    default:
        break;

    }

}
