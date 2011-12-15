#include "fw/helpers/lock.hpp"

Fw::Helpers::Lockable::Lockable()
{

}

Fw::Helpers::Lockable::~Lockable()
{

}

Fw::Helpers::Locker::Locker(Lockable& lockable, const Mode& mode) :
    m_lockable(lockable),
    m_lock(false)
{
    switch(mode)
    {
    case M_ReadLock :
        m_lockable.m_lock.lockForRead();
        m_lock = true;
        break;
    case M_WriteLock :
        m_lockable.m_lock.lockForWrite();
        m_lock = true;
        break;
    case M_TryReadLock :
        m_lock = m_lockable.m_lock.tryLockForRead();
        break;
    case M_TryWriteLock :
        m_lock = m_lockable.m_lock.tryLockForWrite();
        break;
    default :
        break;
    }
}
Fw::Helpers::Locker::~Locker()
{
    if(m_lock)
    {
        m_lockable.m_lock.unlock();
        m_lock = false;
    }
}
