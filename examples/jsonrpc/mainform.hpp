#ifndef MAINFORM_HPP
#define MAINFORM_HPP

#include <QWidget>

namespace Ui {
class MainForm;
}

class MainForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit MainForm(QWidget *parent = 0);
    ~MainForm();
    
private:
    Ui::MainForm *ui;
};

#endif // MAINFORM_HPP
