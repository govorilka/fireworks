#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/qmainwindow.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    typedef QMainWindow BaseClass;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
