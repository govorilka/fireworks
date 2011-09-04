#ifndef TESTDOCUMENTVIEW_H
#define TESTDOCUMENTVIEW_H

#include <QWidget>

namespace Ui {
    class TestDocumentView;
}

namespace Test
{
    class DocumentView;
    class DocumentModel;
}

class Test::DocumentView : public QWidget
{
    Q_OBJECT

public:
    explicit DocumentView(DocumentModel* model, QWidget *parent = 0);
    ~DocumentView();

private:
    Ui::TestDocumentView *ui;
};

#endif // TESTDOCUMENTVIEW_H
