#ifndef FIREWORKS_PG_H
#define FIREWORKS_PG_H

#include <QtCore/qobject.h>

#include "fireworks.h"

namespace FwPg
{
   class Database;
}

class FIREWORKSSHARED_EXPORT FwPg::Database : public QObject
{
    Q_OBJECT
    typedef QObject BaseClass;

public:
    Database(QObject* parent = 0);
    virtual ~Database();

    void open();
};

#endif //FIREWORKS_PG_H
