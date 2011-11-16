#include "fw/database/query.hpp"

Fw::Database::Query::Query(const DriverPtr& driver) :
    m_driver(driver),
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

//=============================================================================

//Fw::Database::QueryData::QueryData(Fw::Database::Controller* controller) :
//    m_controller(controller),
//    m_exec(false)
//{
//    if(m_controller)
//    {
//        m_controller->m_queries.append(this);
//    }
//}

//Fw::Database::QueryData::~QueryData()
//{
//}

//bool Fw::Database::QueryData::step() throw (Fw::Exception&)
//{
//    if(!(m_exec = (m_exec ? doNext() : doExec())))
//    {
//        doReset();
//    }
//    return m_exec;
//}

//void Fw::Database::QueryData::reset()
//{
//    if(m_exec)
//    {
//        doReset();
//        m_exec = false;
//    }
//}



//void Fw::Database::QueryData::release()
//{
//    if(m_controller)
//    {
//        reset();
//        doFinalize();
//        m_controller->m_queries.removeOne(this);
//        m_controller = 0;
//    }
//}

/////////////////////////////////////////////////////////////////////////////////

//Fw::Database::Query::Query(QueryData * data) :
//    BaseClass(data, &doDeleteQueryData)
//{
//}

//Fw::Database::Query::~Query()
//{
//}

//Fw::Database::QueryData* Fw::Database::Query::getQueryData() const throw (Fw::Exception&)
//{
//    QueryData* data = this->data();
//    if(!data || !data->controller())
//    {
//        throw Fw::Exception("Query is null");
//    }
//    return data;
//}

//Fw::Database::QueryData* Fw::Database::Query::getBindQueryData() const throw (Fw::Exception&)
//{
//    QueryData* data = getQueryData();
//    if(data->m_exec)
//    {
//        throw Fw::Exception("Query is execite now");
//    }
//    return data;
//}

//void Fw::Database::Query::doDeleteQueryData(QueryData* data)
//{
//    data->release();
//    delete data;
//    data = 0;
//}
