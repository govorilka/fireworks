#include <QtCore/qdebug.h>
#include "dbwork.h"

DbWork::DbWork()
{
}

bool DbWork::loadDB(const QString& param)
{
#ifdef USE_SQLITE_API
    m_connection = 0;
    m_stmt = 0;
    const int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;

    int result = sqlite3_open_v2(param.toUtf8().data(), &m_connection, flags, 0);
    if(m_connection &&
       result == SQLITE_OK &&
       sqlite3_exec(m_connection, "PRAGMA FOREIGN_KEYS = ON;", 0, 0, 0) == SQLITE_OK &&
       sqlite3_exec(m_connection, "PRAGMA ENCODING=\"UTF-8\";", 0, 0, 0) == SQLITE_OK)
    {
        return true;
    }

    if(m_connection)
    {
        sqlite3_close(m_connection);
        m_connection = 0;
    }
#else
    try
    {
        m_db = Fw::dbFactory(0, param);
        return true;
    }
    catch(Fw::Exception& e)
    {
        qDebug() << e.what();
    }
#endif
    return false;
}

#ifdef USE_SQLITE_API
bool DbWork::createQuery(const QString& query)
{
    if(m_stmt)
    {
        m_stmt = 0;
    }
    if(m_connection)
    {
        QByteArray request = query.toUtf8();
        int result = sqlite3_prepare_v2(m_connection,
                                          request.constData(),
                                          (request.size() + 1) * sizeof(QChar),
                                          &m_stmt,
                                          0);
        if(result == SQLITE_OK)
        {
            return true;
        }
    }
    return false;
}

void DbWork::bindInt(int index, int value)// throw(Fw::Exception&)
{
    if(m_stmt)
    {
        sqlite3_bind_int(m_stmt, index, value);
    }
}

void DbWork::bindText(int index, const QString& text) //throw(Fw::Exception&)
{
    if(m_stmt)
       {
           sqlite3_bind_text16(m_stmt, index, text.utf16(), (text.size()) * sizeof(QChar), SQLITE_TRANSIENT);
       }
}


bool DbWork::columnBool(int column) const
{
    return columnInt(column) != 0;
}

int DbWork::columnInt(int column) const
{
    if(m_stmt)
    {
        return sqlite3_column_int(m_stmt, column);
    }
    return 0;
}

QString DbWork::columnText(int column) const
{
    if(m_stmt)
    {
        return QString( reinterpret_cast<const QChar*>(sqlite3_column_text16(m_stmt, column)),
                        sqlite3_column_bytes16(m_stmt, column) / sizeof(QChar)
                      );
    }
    return QString();
}

bool DbWork::step()
{
    if(m_stmt)
    {
        switch(sqlite3_step(m_stmt))
        {
            case SQLITE_ROW:
            return true;

            case SQLITE_DONE:
            return false;

            case SQLITE_ERROR:
            case SQLITE_IOERR:
            return false;

            default:
            Q_ASSERT(false);
            return false;
        }
    }

    return false;
}

#endif


bool DbWork::selectQuery(int index)
{
    QString request;
    if(index)
    {
        request = QString("SELECT * FROM testtable WHERE rowId = ?1;");
    } else
    {
        request = QString("SELECT * FROM testtable;");
    }
    #ifdef USE_SQLITE_API
    createQuery(request);
    bindInt(1, index);
    while(step())
    {
        int id = columnInt(0);
        QString text = columnText(1);
        qDebug() << "id: " << id << "Text: " << text;
    }
    #else
    Fw::Query query = m_db->query(request);
    query.bindInt(1, index);
    while(query.step())
    {
        int id = query.columnInt(0);
        QString text = query.columnText(1);
        qDebug() << "id: " << id << "Text: " << text;
    }
    #endif
return true;
}

bool DbWork::selectByText(const QString& param)
{
QString request = QString("SELECT * FROM testtable WHERE rowText = ?1;");
#ifdef USE_SQLITE_API
    createQuery(request);
    bindText(1, param);
    while(step())
    {
        int id = columnInt(0);
        QString text = columnText(1);
        qDebug() << "id: " << id << "Text: " << text;
    }
#else
Fw::Query query = m_db->query(request);
query.bindText(1, param);
while(query.step())
{
    int id = query.columnInt(0);
    QString text = query.columnText(1);
    qDebug() << "id: " << id << "Text: " << text;
}
#endif
return true;
}

bool DbWork::insertQuery(const QString& text)
{
    QString request = QString("INSERT INTO testtable(rowText) VALUES(?1);");
    #ifdef USE_SQLITE_API
        createQuery(request);
        bindText(1, text);
        if(step())
        {
            qDebug() << "Success!";
        }
    #else
    Fw::Query query = m_db->query(request);
    query.bindText(1, text);
    if(query.step())
    {
        qDebug() << "Success!";
    }
    #endif
    return true;
}
