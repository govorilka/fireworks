#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

#include "fw/core/json.hpp"

class QTreeWidget;
class QTreeWidgetItem;

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    typedef QMainWindow BaseClass;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void addNode(QTreeWidgetItem* parent, Fw::JSON::Node* node);

public slots:
    void open();
    void open(const QString& fileName);

private:
    Ui::MainWindow *ui;
    Fw::JSON::Object m_rootObject;
};

#endif // MAINWINDOW_H
