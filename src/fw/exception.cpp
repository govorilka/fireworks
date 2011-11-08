
#include "fw/exception.hpp"

Fw::Exception::Exception(const QByteArray& error,
                         const QByteArray& fileName,
                         int line,
                         int  column) throw() :
    BaseClass()
{
    if(fileName.isEmpty())
    {
        m_error = error;
    }
    else
    {
        QByteArray tmp = fileName;
        if(line != -1)
        {
            tmp += " (" + QByteArray::number(line);
            if(column != -1)
            {
                tmp += ", " + QByteArray::number(column);
            }
            tmp += ")";
        }
        m_error = tmp + ": " + error;
    }
}

Fw::Exception::Exception(const QByteArray& error,
                         int line,
                         int  column) throw() :
    BaseClass()
{
    if(line != -1)
    {
        QByteArray tmp = " (" + QByteArray::number(line);
        if(column != -1)
        {
            tmp += ", " + QByteArray::number(column);
        }
        m_error = tmp + "): " + error;
    }
    else
    {
        m_error = error;
    }
}

Fw::Exception::Exception(char c,
                         int line,
                         int column) throw() :
    BaseClass()
{
    m_error = " (" +
              QByteArray::number(line) +
              ", " +
              QByteArray::number(column) +
              "): Unexcepted char '" +
              c +
              "'";
}

Fw::Exception::Exception(const QFile& file) :
    BaseClass(),
    m_error((file.fileName() + ": " + file.errorString()).toUtf8())
{
}

Fw::Exception::~Exception() throw()
{
}

const char* Fw::Exception::what() const throw()
{
    return m_error.constData();
}
