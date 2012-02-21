#include <QtCore/qdebug.h>

#include <QtNetwork/qnetworkaccessmanager.h>

#include <QtGui/qlineedit.h>

#include "fw/jsonrpc/jsonrpc.hpp"

#include "mainform.hpp"
#include "ui_mainform.h"

MainForm::MainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainForm),
    m_networkManager(0),
    m_jsonRpc(0)
{
    ui->setupUi(this);
    ui->responceTree->setColumnCount(3);
    ui->responceTree->setHeaderLabels(QStringList() << "Node" << "Type" << "Value");
    connect(ui->resetBtn, SIGNAL(clicked()), this, SLOT(resetRequest()));
    connect(ui->sendBtn, SIGNAL(clicked()), this, SLOT(sendRequest()));

    m_networkManager = new QNetworkAccessManager(this);

    m_jsonRpc = new Fw::JSON::RPC(m_networkManager, this);
    connect(m_jsonRpc, SIGNAL(finished(Fw::JSON::RPC::Response)), this, SLOT(finish(Fw::JSON::RPC::Response)));
}

MainForm::~MainForm()
{
    delete ui;
}

void MainForm::resetRequest()
{
    Fw::JSON::RPC::Request r;
    r.setID(0);
    r.setMethod("method");
    r.setParam(new Fw::JSON::Object);
    setRequestText(r.toUtf8());
}

void MainForm::sendRequest()
{
    try
    {
        m_jsonRpc->send(QUrl(ui->urlEdit->text()), ui->requestEdit->toPlainText());
    }
    catch(const Fw::Exception& e)
    {
        qDebug() << e.error();
    }
}

QByteArray MainForm::requestText() const
{
    return ui->requestEdit->toPlainText().toUtf8();
}

void MainForm::setRequestText(const QByteArray& text)
{
    ui->requestEdit->setPlainText(QString::fromUtf8(text));
}

void MainForm::finish(const Fw::JSON::RPC::Response& r)
{
    ui->responceSource->setPlainText(r.toUtf8());

    ui->responceTree->clear();

    QTreeWidgetItem* rootItem = new QTreeWidgetItem(ui->responceTree);
    rootItem->setText(0, "root");
    addNode(rootItem, r.object().data());
}

void MainForm::addNode(QTreeWidgetItem* parent, Fw::JSON::Node* node)
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
