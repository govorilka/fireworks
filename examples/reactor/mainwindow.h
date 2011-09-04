#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/qtextdocument.h>
#include <QtGui/qtextformat.h>

#include <QMainWindow>

namespace Ui
{
    class MainWindow;
}

class Database;
class DataNode;
class DataEdit;

class QPrinter;

namespace Test
{
    class Document;
    class DocumentModel;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    typedef QMainWindow BaseClass;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void documentNew();

    void print();

private:
    void mergeFormatOnWordOrSelection(const QTextCharFormat& fmt);

    Ui::MainWindow *ui;

    QPrinter* m_printer;

    Database* m_db;
    DataEdit* m_currentEdit;

    Test::DocumentModel* m_testDocument;

private slots:
    void currentChanged(DataNode* node);

    void textEditBold();
    void textEditItanic();
    void textEditUnderline();
};

#endif // MAINWINDOW_H
