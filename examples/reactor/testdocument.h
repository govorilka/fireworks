#ifndef TESTDOCUMENT_H
#define TESTDOCUMENT_H

#include <QtCore/qobject.h>
#include <QtCore/qpointer.h>

#include "fireworks.h"

class FwSQLiteDatabase;

namespace Test
{
    class Document;
    class Question;
}

class Test::Question
{
public:

    friend class Document;

    Question();

    inline bool isNull() const;

    inline int id() const;
    inline int index() const;

protected:
    Question(Document* document, int id, int index);

private:
    QPointer<Document> m_document;

    int m_id;
    int m_index;
};

bool Test::Question::isNull() const
{
    return m_document.isNull() || !m_id;
}

int Test::Question::id() const
{
    return m_id;
}

int Test::Question::index() const
{
    return m_index;
}

////////////////////////////////////////////////////////////////////////////////

class Test::Document : public QObject
{
    Q_OBJECT
    typedef QObject BaseClass;
public:
    Document(QObject* parent = 0);
    virtual ~Document();

    bool open(const QString& fileName, QString* errorMessage = 0);
    void close();

    QVector<Question> questions() const;

private:
    FwSQLiteDatabase* m_database;
};

#endif // TESTDOCUMENT_H
