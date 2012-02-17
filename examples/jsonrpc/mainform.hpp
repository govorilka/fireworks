#ifndef MAINFORM_HPP
#define MAINFORM_HPP

#include <QtGui/qwidget.h>

#include "fw/jsonrpc/jsonrpc.hpp"

namespace Ui
{
    class MainForm;
}

class MainForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit MainForm(QWidget *parent = 0);
    ~MainForm();

    QByteArray requestText() const;
    void setRequestText(const QByteArray& text);

public slots:
    void resetRequest();

private:
    Ui::MainForm *ui;
};

#endif // MAINFORM_HPP
