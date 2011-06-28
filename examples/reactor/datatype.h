#ifndef REACTOR_DATATYPE_H
#define REACTOR_DATATYPE_H

#include <QtCore/qstring.h>

#include <QtGui/qicon.h>

class QWidget;

class Database;

class DataNode;
class DataEdit;

class DataTypeRoot;
class DataTypeTheme;
class DataTypeQuestion;

class DataType
{
public:
    DataType(Database* db, const QString& className);
    virtual ~DataType();

    inline Database* db() const;
    inline QString className() const;
    inline QIcon icon() const;

    inline DataEdit* edit() const;
    inline void setEdit(DataEdit* edit);

    virtual DataEdit* createEdit(QWidget* parent) = 0;

    virtual bool hasChildren() const;
    virtual QVector<DataType*> childrenTypes() const;
    virtual void loadChildren(DataNode* node);

    DataNode* addNode(DataNode* parent);

protected:

    virtual bool dataInsert(DataNode* parent, int& key, QString& caption) = 0;

    void removeNode(DataNode* node);

private:
    Database* m_db;
    QString m_className;
    QIcon m_icon;
    DataEdit* m_edit;
};

Database* DataType::db() const
{
    return m_db;
}

QString DataType::className() const
{
    return m_className;
}

QIcon DataType::icon() const
{
    return m_icon;
}

DataEdit* DataType::edit() const
{
    return m_edit;
}

void DataType::setEdit(DataEdit* edit)
{
    m_edit = edit;
}

/////////////////////////////////////////////////////////////////////////////

class DataTypeRoot : public DataType
{
    typedef DataType BaseClass;

public:
    DataTypeRoot(Database* db);

    DataEdit* createEdit(QWidget* parent);

    bool hasChildren() const;
    QVector<DataType*> childrenTypes() const;
    void loadChildren(DataNode* node);

    inline DataTypeTheme* typeTheme() const;
    inline void setDataTypeTheme(DataTypeTheme* theme);

    inline DataTypeQuestion* typeQuestion() const;
    inline void setDataTypeQuestion(DataTypeQuestion* question);

protected:
    bool dataInsert(DataNode* parent, int& key, QString& caption);

private:
    DataTypeTheme* m_typeTheme;
    DataTypeQuestion* m_typeQuestion;
};

DataTypeTheme* DataTypeRoot::typeTheme() const
{
    return m_typeTheme;
}

void DataTypeRoot::setDataTypeTheme(DataTypeTheme* theme)
{
    m_typeTheme = theme;
}

DataTypeQuestion* DataTypeRoot::typeQuestion() const
{
    return m_typeQuestion;
}

void DataTypeRoot::setDataTypeQuestion(DataTypeQuestion* question)
{
    m_typeQuestion = question;
}

/////////////////////////////////////////////////////////////////////////////

class DataTypeTheme : public DataType
{
    typedef DataType BaseClass;

public:
    DataTypeTheme(Database* db);

    DataEdit* createEdit(QWidget* parent);

protected:
    bool dataInsert(DataNode* parent, int& key, QString& caption);
};

/////////////////////////////////////////////////////////////////////////////

class DataTypeQuestion : public DataType
{
    typedef DataType BaseClass;

public:
    DataTypeQuestion(Database* db);

    DataEdit* createEdit(QWidget* parent);

protected:
    bool dataInsert(DataNode* parent, int& key, QString& captionn);
};

#endif // REACTOR_DATATYPE_H
