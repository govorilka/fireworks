#ifndef FIREWORKS_DATABASE_CONSTRUCTOR_HPP
#define FIREWORKS_DATABASE_CONSTRUCTOR_HPP

#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>

#include "fw/database/defs.hpp"

class FIREWORKSSHARED_EXPORT Fw::Database::Constructor
{
public:
    Constructor();

private:
    QStringList m_fields;
    QString m_primaryKey;
};

#endif // FIREWORKS_DATABASE_CONSTRUCTOR_HPP
