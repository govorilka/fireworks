//#include "querydata.hpp"
//#include "driver.hpp"

//Fw::Database::SQLite::QueryData::QueryData(Controller* controller, sqlite3_stmt* stmt) :
//    BaseClass(controller),
//    m_stmt(stmt)
//{
//}

//Fw::Database::SQLite::QueryData::~QueryData()
//{
//}

//bool Fw::Database::SQLite::QueryData::isNull() const
//{
//    return m_stmt == 0;
//}

//bool Fw::Database::SQLite::QueryData::doExec() throw (Fw::Exception&)
//{
//    return doNext();
//}

//bool Fw::Database::SQLite::QueryData::doNext() throw (Fw::Exception&)
//{
//    if(m_stmt)
//    {
//        switch(sqlite3_step(m_stmt))
//        {
//            case SQLITE_ROW:
//            return true;

//            case SQLITE_DONE:
//            return false;

//            case SQLITE_ERROR:
//            case SQLITE_IOERR:
//            case SQLITE_CONSTRAINT:
//            throw Fw::Exception(static_cast<Driver*>(driver())->lastError().toUtf8());
//            return false;

//            default:
//            Q_ASSERT(false);
//            return false;
//        }
//    }

//    throw Fw::Exception("Query is null");
//    return false;
//}

//void Fw::Database::SQLite::QueryData::doReset()
//{
//    if(m_stmt)
//    {
//        sqlite3_reset(m_stmt);
//    }
//}

//void Fw::Database::SQLite::QueryData::doFinalize()
//{
//    if(m_stmt)
//    {
//        sqlite3_finalize(m_stmt);
//        m_stmt = 0;
//    }
//}

//void Fw::Database::SQLite::QueryData::doBindInt(int index, int value) throw(Fw::Exception&)
//{
//    if(m_stmt)
//    {
//        sqlite3_bind_int(m_stmt, index, value);
//    }
//}
//void Fw::Database::SQLite::QueryData::doBindText(int index, const QString& text) throw(Fw::Exception&)
//{
//    if(m_stmt)
//       {
//           sqlite3_bind_text16(m_stmt, index, text.utf16(), (text.size()) * sizeof(QChar), SQLITE_TRANSIENT);
//       }
//}
//void Fw::Database::SQLite::QueryData::doBindDateTime(int index, const QDateTime& dateTime) throw(Fw::Exception&)
//{
//    doBindInt(index, static_cast<int>(dateTime.toUTC().toTime_t()));
//}

//void Fw::Database::SQLite::QueryData::doBindDate(int index, const QDate& date) throw(Fw::Exception&)
//{
//    doBindDateTime(index, QDateTime(date));
//}

//void Fw::Database::SQLite::QueryData::doBindTime(int index, const QTime& time) throw(Fw::Exception&)
//{
//    doBindInt(index, time.second() + time.minute() * 60 + time.hour() * 3600);
//}

//bool Fw::Database::SQLite::QueryData::doColumnBool(int column) const
//{
//    return doColumnInt(column) != 0;
//}

//int Fw::Database::SQLite::QueryData::doColumnInt(int column) const
//{
//    if(m_stmt)
//    {
//        return sqlite3_column_int(m_stmt, column);
//    }
//    return 0;
//}

//QString Fw::Database::SQLite::QueryData::doColumnText(int column) const
//{
//    if(m_stmt)
//    {
//        return QString( reinterpret_cast<const QChar*>(sqlite3_column_text16(m_stmt, column)),
//                        sqlite3_column_bytes16(m_stmt, column) / sizeof(QChar)
//                      );
//    }
//    return QString();
//}

//FwColor Fw::Database::SQLite::QueryData::doColumnColor(int column) const
//{
//    return FwColor(static_cast<quint32>(doColumnInt(column)));
//}

//QUrl Fw::Database::SQLite::QueryData::doColumnUrl(int column) const
//{
//    QString stringUrl = doColumnText(column);
//    if(!stringUrl.isEmpty())
//    {
//        return QUrl(stringUrl);
//    }
//    return QUrl();
//}

//QDateTime Fw::Database::SQLite::QueryData::doColumnDateTime(int column) const
//{
//    return QDateTime::fromTime_t(doColumnInt(column));
//}

//QDate Fw::Database::SQLite::QueryData::doColumnDate(int column) const
//{
//    return QDateTime::fromTime_t(doColumnInt(column)).date();
//}

//QTime Fw::Database::SQLite::QueryData::doColumnTime(int column) const
//{
//    int time = doColumnInt(column);
//    int h = time / 3600;
//    time -= h * 3600;
//    int m = time / 60;
//    int s = time - m * 60;
//    return QTime(h, m, s);
//}
