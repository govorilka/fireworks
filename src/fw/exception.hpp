#ifndef FIREWORKS_EXCEPTION_H
#define FIREWORKS_EXCEPTION_H

#include <exception>

#include <QtCore/qfile.h>

#include "fw/defs.hpp"

class FIREWORKSSHARED_EXPORT Fw::Exception : public std::exception
{
    typedef std::exception BaseClass;

public:
    Exception(const QByteArray& error,
              const QByteArray& fileName = QByteArray(),
              int line = -1,
              int  column = -1) throw();

    Exception(const QByteArray& error,
              int line,
              int  column) throw();

    Exception(char c,
              int line,
              int  column) throw();

    Exception(const QFile& file);

    virtual ~Exception() throw();

    virtual const char* what() const throw();

    inline QByteArray error() const;

protected:
    QByteArray m_error;
};

QByteArray Fw::Exception::error() const
{
    return m_error;
}


#endif // FIREWORKS_EXCEPTION_H
