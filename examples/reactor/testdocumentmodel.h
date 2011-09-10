#ifndef TESTDOCUMENTMODEL_H
#define TESTDOCUMENTMODEL_H

#include <QtGui/qabstractitemview.h>
#include <QtCore/qvector.h>

#include "testdocument.h"

namespace Test
{
    enum NodeType
    {
        NT_Root,
        NT_Question,
    };

    class DocumentModel;

    class DocumentModelNode;

    class DocumentModelType;
    class DocumentModelTypeRoot;
    class DocumentModelTypeQuestion;
}

////////////////////////////////////////////////////////////////////////////////

class Test::DocumentModelType
{
public:
    DocumentModelType(DocumentModel* model);
    virtual ~DocumentModelType();

    inline DocumentModel* model() const;

    virtual NodeType typeID() const = 0;

    virtual QIcon icon(DocumentModelNode* node) const = 0;
    virtual QString caption(DocumentModelNode* node) const = 0;

    virtual bool hasChildren() const = 0;
    virtual void loadChildren(DocumentModelNode* parentNode) const = 0;
    virtual void createChild(DocumentModelNode* parentNode) = 0;
    virtual void removeNode(DocumentModelNode* node) = 0;

private:
    DocumentModel* m_model;
};

Test::DocumentModel* Test::DocumentModelType::model() const
{
    return m_model;
}

/////////////////////////////////////////////////////////////////////////////

class Test::DocumentModelTypeRoot : public Test::DocumentModelType
{
    typedef Test::DocumentModelType BaseClass;

public:
    DocumentModelTypeRoot(DocumentModel* model);

    virtual NodeType typeID() const;

    virtual QIcon icon(DocumentModelNode* node) const;
    virtual QString caption(DocumentModelNode* node) const;

    virtual bool hasChildren() const;
    virtual void loadChildren(DocumentModelNode* parentNode) const;
    virtual void createChild(DocumentModelNode* parentNode);
    virtual void removeNode(DocumentModelNode* node);

private:
    QIcon m_icon;
};

/////////////////////////////////////////////////////////////////////////////

class Test::DocumentModelTypeQuestion : public Test::DocumentModelType
{
    typedef Test::DocumentModelType BaseClass;

public:
    DocumentModelTypeQuestion(DocumentModel* model);

    virtual NodeType typeID() const;

    virtual QIcon icon(DocumentModelNode* node) const;
    virtual QString caption(DocumentModelNode* node) const;

    virtual bool hasChildren() const;
    virtual void loadChildren(DocumentModelNode* parentNode) const;
    virtual void createChild(DocumentModelNode* parentNode);
    virtual void removeNode(DocumentModelNode* node);;

private:
    QIcon m_icon;
};

////////////////////////////////////////////////////////////////////////////////

class Test::DocumentModelNode
{
public:
    friend class DocumentModel;

    DocumentModelNode();
    DocumentModelNode(DocumentModelType* type, DocumentModelNode* parent, int key);
    ~DocumentModelNode();

    inline DocumentModelType* type() const;
    inline DocumentModelNode* parent() const;
    inline int key() const;

    inline QList<DocumentModelNode*> children() const;

private:
    QList<DocumentModelNode*> m_children;
    DocumentModelType* m_type;
    DocumentModelNode* m_parent;
    int m_key;
};

QList<Test::DocumentModelNode*> Test::DocumentModelNode::children() const
{
    return m_children;
}

Test::DocumentModelType* Test::DocumentModelNode::type() const
{
    return m_type;
}

Test::DocumentModelNode* Test::DocumentModelNode::parent() const
{
    return m_parent;
}

int Test::DocumentModelNode::key() const
{
    return m_key;
}

////////////////////////////////////////////////////////////////////////////////

class Test::DocumentModel : public QAbstractItemModel
{
    Q_OBJECT;
    typedef QAbstractItemModel BaseClass;

public:
    DocumentModel(QObject* parent = 0);
    ~DocumentModel();

    inline Document* document() const;

    bool open(const QString& fileName, QString* errorMessage = 0);
    void close();

    //Implemention QAbstractItemModel class
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    inline static DocumentModelNode* node(const QModelIndex& index);
    QModelIndex createIndex(DocumentModelNode* node) const;

private:
    Document* m_document;
    DocumentModelNode* m_rootNode;

    DocumentModelTypeRoot* m_typeRoot;
    DocumentModelTypeQuestion* m_typeQuestion;
};

Test::Document* Test::DocumentModel::document() const
{
    return m_document;
}

Test::DocumentModelNode* Test::DocumentModel::node(const QModelIndex& index)
{
    if(index.isValid())
    {
        return static_cast<DocumentModelNode*>(index.internalPointer());
    }
    return 0;
}

#endif // TESTDOCUMENTMODEL_H
