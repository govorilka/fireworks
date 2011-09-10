#include "testdocumentview.h"
#include "testdocumentmodel.h"

#include "ui_testdocumentview.h"

Test::DocumentView::DocumentView(DocumentModel* model, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestDocumentView)
{
    ui->setupUi(this);

    ui->tree->setModel(model);
}

Test::DocumentView::~DocumentView()
{
    delete ui;
}
