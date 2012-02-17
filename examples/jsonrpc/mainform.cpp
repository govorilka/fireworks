#include "fw/jsonrpc/jsonrpc.hpp"

#include "mainform.hpp"
#include "ui_mainform.h"

MainForm::MainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainForm)
{
    ui->setupUi(this);
    connect(ui->resetBtn, SIGNAL(clicked()), this, SLOT(resetRequest()));
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
    setRequestText(r.toUtf8());
}

QByteArray MainForm::requestText() const
{
    return ui->requestEdit->toPlainText().toUtf8();
}

void MainForm::setRequestText(const QByteArray& text)
{
    ui->requestEdit->setPlainText(QString::fromUtf8(text));
}
