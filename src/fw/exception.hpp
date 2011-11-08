#ifndef FIREWORKS_EXCEPTION_H
#define FIREWORKS_EXCEPTION_H

#include <QtCore/qfile.h>

#include "fireworks.h"

class FIREWORKSSHARED_EXPORT Fw::Exception : public std::exception
{
    typedef std::exception BaseClass;

public:
    Exception(const QByteArray& error,
              const QByteArray& fileName = QByteArray(),
              int line = -1,
              int  column = -1) throw();

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

class Exception
{
public:
    Exception();
};

#endif // FIREWORKS_EXCEPTION_H
