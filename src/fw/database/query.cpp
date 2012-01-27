#include "fw/database/query.hpp"

Fw::Database::Query::Query(const DriverLoaderPtr& driver) :
    m_driverLoader(driver),
    m_exec(false)
{
}

Fw::Database::Query::~Query()
{
}

bool Fw::Database::Query::step() throw (const Fw::Exception&)
{
    if(!(m_exec = (m_exec ? doNext() : doExec())))
    {
        doReset();
    }
    return m_exec;
}

void Fw::Database::Query::reset()
{
    if(m_exec)
    {
        doReset();
        m_exec = false;
    }
}

void Fw::Database::Query::bindBool(int index, bool value) throw(const Fw::Exception&)
{
    bindInt(index, value);
}

void Fw::Database::Query::bindUrl(int index, const QUrl& url) throw(const Fw::Exception&)
{
    bindText(index, url.toString());
}
