#include "selectdatatypedialog.h"
#include "ui_selectdatatypedialog.h"

#include "datatype.h"

SelectDataTypeDialog::SelectDataTypeDialog(QWidget *parent, const QVector<DataType*>& types, DataType* current) :
    QDialog(parent),
    ui(new Ui::SelectDataTypeDialog),
    m_types(types)
{
    ui->setupUi(this);

    int index = 0;
    int currentIndex = 0;
    foreach(DataType* type, m_types)
    {
        ui->comboBox->addItem(type->icon(), type->className());
        if(current && current == type)
        {
            currentIndex = index;
        }
        index++;
    }
    ui->comboBox->setCurrentIndex(currentIndex);
}

SelectDataTypeDialog::~SelectDataTypeDialog()
{
    delete ui;
}

DataType* SelectDataTypeDialog::currentDataType() const
{
    int index = ui->comboBox->currentIndex();
    if(index != -1 && index < m_types.size())
    {
        return m_types[index];
    }
    return 0;
}

DataType* SelectDataTypeDialog::getType(QWidget* parent, const QVector<DataType*>& types, DataType* current)
{
    if(!types.isEmpty())
    {
        DataType* result = 0;

        SelectDataTypeDialog* dlg = new SelectDataTypeDialog(parent, types, current);
        if(dlg->exec() == QDialog::Accepted)
        {
            result = dlg->currentDataType();
        }
        dlg->deleteLater();
        return result;
    }

    return 0;
}
