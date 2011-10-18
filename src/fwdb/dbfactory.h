#include "fwdb/fwdb.h"

namespace Fw
{
    Database* dbFactory(QObject* parent, const QString& driver, const QString& params) throw(Exception&);
    Database* dbFactory(QObject* parent, const QString& configFile) throw(Fw::Exception&);
    class DbConfig;
}

class FIREWORKSSHARED_EXPORT Fw::DbConfig : public FwCPPObject
{
    typedef FwCPPObject BaseClass;
public:
    DbConfig();
    virtual bool loadData(FwMLObject* object);

    inline QString driver() const;
    inline QString connectionParameters() const;
private:
    QString m_driver;
    QString m_connectionParameters;
};

inline QString Fw::DbConfig::driver() const
{
    return m_driver;
}

inline QString Fw::DbConfig::connectionParameters() const
{
    return m_connectionParameters;
}
