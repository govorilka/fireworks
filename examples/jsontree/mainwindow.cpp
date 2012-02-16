#include <QtCore/qdebug.h>

#include <QtGui/qtreewidget.h>
#include <QtGui/qfiledialog.h>

#include "fw/core/exception.hpp"

#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    BaseClass(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralwidget->setColumnCount(3);
    ui->centralwidget->setHeaderLabels(QStringList() << "Node" << "Type" << "Value");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addNode(QTreeWidgetItem* parent, Fw::JSON::Node* node)
{
    switch(node->type())
    {
    case Fw::JSON::T_Object:
        {
            parent->setText(1, "object");

            Fw::JSON::Object* object = node->cast<Fw::JSON::Object>();
            QHash<QByteArray, Fw::JSON::Node*> attributes = object->attributes();
            for(QHash<QByteArray, Fw::JSON::Node*>::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter)
            {
                QTreeWidgetItem* childItem = new QTreeWidgetItem(parent);
                childItem->setText(0, QString::fromUtf8(iter.key()));
                addNode(childItem, iter.value());
            }

            QString str = "<empty>";
            if(object->hasValue<Fw::JSON::String>("stroka", &str))
            {
                qDebug() << str;
            }
            qDebug() << object->value<Fw::JSON::String>("stroka");

        }
        break;

    case Fw::JSON::T_String:
        {
            bool bOk = false;
            parent->setText(1, "string");
            Fw::JSON::String* stringNode = node->cast<Fw::JSON::String>();
            parent->setText(2, stringNode->isEmpty() ? "<empty>" : stringNode->toString(&bOk));
        }
        break;

    case Fw::JSON::T_Number:
        {
            parent->setText(1, "number");
            parent->setText(2, QString::fromUtf8(node->toUtf8()));
        }
        break;

    case Fw::JSON::T_Array:
        {
            Fw::JSON::Array* array = node->cast<Fw::JSON::Array>();

            parent->setText(1, QString("array[%1]").arg(array->size()));

            int i = 0;
            foreach(Fw::JSON::Node* child, array->toQVector())
            {
                QTreeWidgetItem* childItem = new QTreeWidgetItem(parent);
                childItem->setText(0, QString("[%1]").arg(i++));
                addNode(childItem, child);
            }
        }
        break;
    case Fw::JSON::T_Bool:
        {
            parent->setText(1, "boolean");
            parent->setText(2, QString::fromUtf8(node->toUtf8()));
        }
        break;
    default:
        break;

    }

}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open fwml file"));
    if(!fileName.isEmpty())
    {
        open(fileName);
    }
}

void MainWindow::open(const QString& fileName)
{
    m_rootObject.clear();
    ui->centralwidget->clear();

    QTreeWidgetItem* rootItem = new QTreeWidgetItem(ui->centralwidget);
    rootItem->setText(0, "root");

    try
    {
         m_rootObject.parseFile(fileName);
         addNode(rootItem, &m_rootObject);
    }
    catch(const Fw::Exception& e)
    {
        QTreeWidgetItem* errorItem = new QTreeWidgetItem(rootItem);
        errorItem->setText(0, "Error");
        errorItem->setText(1, QString::fromUtf8(e.error()));
    }

}
