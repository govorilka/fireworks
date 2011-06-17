#ifndef DATANODE_H
#define DATANODE_H

#include <QtCore/qstring.h>
#include <QtCore/qvector.h>

class DataType;

class DataNode
{
public:
    DataNode();
    DataNode(DataType* type, int key, const QString& caption = QString());
    ~DataNode();

    inline DataType* type() const;
    inline int key() const;
    inline DataNode* parent() const;
    inline int row() const;

    inline QList<DataNode*> children() const;

    DataNode* createChild(DataType* type, int key, const QString& caption = QString());
    void deleteChild(DataNode* node);

    inline QString caption() const;
    inline void setCaption(const QString& caption);

    DataNode* childByCaption(const QString& caption) const;
    QString nextChildCaption(const QString& caption) const;

private:
    int m_row;
    int m_key;
    DataNode* m_parent;
    DataType* m_type;
    QString m_caption;
    QList<DataNode*> m_children;
};

DataType* DataNode::type() const
{
    return m_type;
}

int DataNode::key() const
{
    return m_key;
}

DataNode* DataNode::parent() const
{
    return m_parent;
}

int DataNode::row() const
{
    return m_row;
}

QList<DataNode*> DataNode::children() const
{
    return m_children;
}

QString DataNode::caption() const
{
    return m_caption;
}

void DataNode::setCaption(const QString& caption)
{
    m_caption = caption;
}

#endif // DATANODE_H
