#ifndef REACTOR_DATAEDIT_H
#define REACTOR_DATAEDIT_H

#include <QtGui/qwidget.h>

namespace Ui
{
    class DataEditRoot;
    class DataEditQuestion;
}

class DataType;
class DataTypeRoot;
class DataTypeQuestion;
class DataTypeTheme;

class DataNode;

class DataEdit : public QWidget
{
    Q_OBJECT
    typedef QWidget BaseClass;

public:
    explicit DataEdit(DataType* type, QWidget *parent = 0);

    inline DataType* type() const;

    void open(DataNode* node);
    void save();
    void close();

private:
    virtual void loadData(DataNode* node);
    virtual void saveData(DataNode* node);

private:
    DataType* m_type;
    DataNode* m_currentNode;
};

///////////////////////////////////////////////////////////////////////////////

class DataEditRoot : public DataEdit
{
    Q_OBJECT
    typedef DataEdit BaseClass;

public:
    explicit DataEditRoot(DataTypeRoot* type, QWidget *parent = 0);
    virtual ~DataEditRoot();

private:
    Ui::DataEditRoot *ui;

};

///////////////////////////////////////////////////////////////////////////////

class DataEditQuestion : public DataEdit
{
    Q_OBJECT
    typedef DataEdit BaseClass;

public:
    explicit DataEditQuestion(DataTypeQuestion* type, QWidget *parent = 0);
    virtual ~DataEditQuestion();

private:
    Ui::DataEditQuestion *ui;
};


#endif // REACTOR_DATAEDIT_H
