#include "fwdb/fwdb.h"

namespace Fw
{
    Database* dbFactory(QObject* parent, const QString& driver, const QString& params) throw(Exception&);
}
