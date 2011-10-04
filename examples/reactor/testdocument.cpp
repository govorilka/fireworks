#include <QtCore/qdebug.h>

#include "fwdb/fwsqlite.h"

#include "testdocument.h"

Test::Question::Question() :
    m_id(0),
    m_index(0)
{
}

Test::Question::Question(Document* document, int id, int index) :
    m_document(document),
    m_id(id),
    m_index(index)
{
}

////////////////////////////////////////////////////////////////////////////////

Test::Document::Document(QObject* parent) :
    BaseClass(parent),
    m_database(0)
{
    m_database = new FwSQLiteDatabase(this);
}

Test::Document::~Document()
{
    close();
}

bool Test::Document::open(const QString& fileName, QString* errorMessage)
{
    close();

    try
    {
        m_database->open(fileName, ":/reactor/sql/init.sql");
        return true;
    }
    catch(FwSQLiteException& e)
    {
        if(errorMessage)
        {
            (*errorMessage) = e.error;
        }
    }

    return false;
}

void Test::Document::close()
{
    m_database->close();
}

QVector<Test::Question> Test::Document::questions() const
{
    if(!m_database->isOpen())
    {
        try
        {
            Document* thisDocument = const_cast<Document*>(this);
            QVector<Question> vector;
            FwSQLiteQuery query = m_database->query("SELECT questionid, questionindex FROM questions");
            while(query.step())
            {
                vector.append(Question(thisDocument, query.columnInt(0), query.columnInt(1)));
            }
            return vector;
        }
        catch(FwSQLiteException& e)
        {
            qDebug() << "Test::Document::questions:" << e.error;
        }
    }
    return QVector<Test::Question>();
}
