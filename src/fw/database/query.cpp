#include "query.hpp"

Fw::Database::QueryData::QueryData(Driver* driver) :
    m_driver(driver),
    m_exec(false)
{
}
