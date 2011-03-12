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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void mergeFormatOnWordOrSelection(const QTextCharFormat& fmt);

    Ui::MainWindow *ui;
    Database* m_db;
    int m_questionId;

private slots:
    void currentChanged(DataNode* node);

    void textEditBold();
    void textEditItanic();
    void textEditUnderline();
};

#endif // MAINWINDOW_H
