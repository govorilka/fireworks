#ifndef SELECTDATATYPEDIALOG_H
#define SELECTDATATYPEDIALOG_H

#include <QDialog>

namespace Ui
{
    class SelectDataTypeDialog;
}

class DataType;

class SelectDataTypeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectDataTypeDialog(QWidget *parent, const QVector<DataType*>& types, DataType* current);
    ~SelectDataTypeDialog();

    inline DataType* currentDataType() const;

    static DataType* getType(QWidget* parent, const QVector<DataType*>& types, DataType* current = 0);

private:
    Ui::SelectDataTypeDialog *ui;
    QVector<DataType*> m_types;
};

#endif // SELECTDATATYPEDIALOG_H
