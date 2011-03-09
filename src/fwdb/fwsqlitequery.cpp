#include "fwsqlitequery.h"
#include "fwsqlitedatabase.h"

FwSQLiteException::FwSQLiteException(const QString& err) throw() :
    BaseClass(),
    error(err)
{
}

FwSQLiteException::FwSQLiteException(sqlite3 *db) throw() :
    BaseClass()
{
    error = db ?
       QString::fromUtf8(sqlite3_errmsg(db)) : "Database is not open";
}

FwSQLiteException::~FwSQLiteException() throw()
{
}

const char* FwSQLiteException::what() const throw()
{
    return qPrintable(error);
}

////////////////////////////////////////////////////////////////////////////////

FwSQLiteQueryData::FwSQLiteQueryData(FwSQLiteDatabase* parent, sqlite3 *pDB, sqlite3_stmt* pStmt) :
    m_parent(0),
    db(0),
    stmt(pStmt)
{
    if(m_parent && !m_parent->queries.contains(this))
    {
        m_parent->queries.append(this);
        m_parent = parent;
        db = pDB;
    }
}

FwSQLiteQueryData::~FwSQLiteQueryData()
{
    finalize();

    if(m_parent)
    {
        m_parent->queries.removeOne(this);
        m_parent = 0;
        db = 0;
    }
}

void FwSQLiteQueryData::finalize()
{
    if(stmt)
    {
        sqlite3_finalize(stmt);
        stmt = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////

/*!
\note Used for null query only
*/
FwSQLiteQuery::FwSQLiteQuery() :
    BaseClass()
{
}

FwSQLiteQuery::FwSQLiteQuery(FwSQLiteQueryData * data) :
    BaseClass(data)
{
}

bool FwSQLiteQuery::isNull() const
{
    FwSQLiteQueryData* data = this->data();
    if(data)
    {
        return data->stmt == 0;
    }
    return true;
}

bool FwSQLiteQuery::operator==(const FwSQLiteQuery &other) const
{
    return data() == other.data();
}

bool FwSQLiteQuery::operator!=(const FwSQLiteQuery &other) const
{
    return data() != other.data();
}

bool FwSQLiteQuery::step() throw (FwSQLiteException&)
{
    FwSQLiteQueryData* data = this->data();
    if(data && data->stmt)
    {
        switch(sqlite3_step(data->stmt))
        {
        case SQLITE_ROW:
            return true;

        case SQLITE_DONE:
            return false;

        case SQLITE_ERROR:
            throw FwSQLiteException(data->db);
            return false;



        default:
            Q_ASSERT(false);
            return false;
        }
    }

    throw FwSQLiteException("Query is null");
    return false;
}

QString FwSQLiteQuery::columnText(int column)
{
    FwSQLiteQueryData* data = this->data();
    if(data && data->stmt)
    {
        return QString(reinterpret_cast<const QChar *>(sqlite3_column_text16(data->stmt, column)),
                       sqlite3_column_bytes16(data->stmt, column) / sizeof(QChar));
    }
    return QString();
}

int FwSQLiteQuery::columnInt(int column)
{
    FwSQLiteQueryData* data = this->data();
    if(data && data->stmt)
    {
        return sqlite3_column_int(data->stmt, column);
    }
    return 0;
}

QUrl FwSQLiteQuery::columnUrl(int column)
{
    QString stringUrl = columnText(column);
    if(!stringUrl.isEmpty())
    {
        return QUrl(stringUrl);
    }
    return QUrl();
}

bool FwSQLiteQuery::columnBool(int column)
{
    return columnInt(column) != 0;
}

QColor FwSQLiteQuery::columnColor(int column)
{
    return QColor(static_cast<quint32>(columnInt(column)));
}

void FwSQLiteQuery::finalize()
{
    FwSQLiteQueryData* data = this->data();
    if(data)
    {
        data->finalize();
    }
}

void FwSQLiteQuery::bindText(int index, const QString& text)
{
    FwSQLiteQueryData* data = this->data();
    if(data && data->stmt)
    {
        sqlite3_bind_text16(data->stmt, index, text.utf16(), (text.size()) * sizeof(QChar), SQLITE_TRANSIENT);
    }
}

void FwSQLiteQuery::bindInt(int index, int value)
{
    FwSQLiteQueryData* data = this->data();
    if(data && data->stmt)
    {
        sqlite3_bind_int(data->stmt, index, value);
    }
}

void FwSQLiteQuery::bindDateTime(int index, const QDateTime& datetime)
{
    FwSQLiteQueryData* data = this->data();
    if(data && data->stmt)
    {
        sqlite3_bind_int(data->stmt, index, (int)datetime.toUTC().toTime_t());
    }
}

void  FwSQLiteQuery::bindColor(int index, const  QColor& color)
{
    FwSQLiteQueryData* data = this->data();
    if(data && data->stmt)
    {
        sqlite3_bind_int(data->stmt, index, static_cast<qint32>(color.rgba()));
    }
}

void FwSQLiteQuery::reset()
{
    FwSQLiteQueryData* data = this->data();
    if(data && data->stmt)
    {
        sqlite3_reset(data->stmt);
    }
}
