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
                      "\"background\" : \"123.png\"; \n"
                      "\"size\" : \"1000 x 1000\", \n"
                      "}";

    QTreeWidgetItem* rootItem = new QTreeWidgetItem(m_treeView);
    rootItem->setText(0, "root");

    m_rootObject = new FwMLObject();
    m_rootObject->parse(fwml);

    addNode(rootItem, m_rootObject);
}

MainWindow::~MainWindow()
{
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

    default:
        break;

    }

}
