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

class QPrinter;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    typedef QMainWindow BaseClass;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void print();

private:
    void mergeFormatOnWordOrSelection(const QTextCharFormat& fmt);

    Ui::MainWindow *ui;
    Database* m_db;
    int m_questionId;
    QPrinter* m_printer;

private slots:
    void currentChanged(DataNode* node);

    void textEditBold();
    void textEditItanic();
    void textEditUnderline();
};

#endif // MAINWINDOW_H
