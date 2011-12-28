#ifndef FIREWORKS_HELPERS_LOCK_HPP
#define FIREWORKS_HELPERS_LOCK_HPP

#include <QtCore/qreadwritelock.h>
#include <QtCore/qdebug.h>

#include "fw/defs.hpp"
#include "fw/helpers/defs.hpp"

class FIREWORKSSHARED_EXPORT Fw::Helpers::Lockable
{
    Q_DISABLE_COPY(Lockable)

    friend class Locker;
    mutable QReadWriteLock m_lock;

protected:
    Lockable();
    virtual ~Lockable();
};

class FIREWORKSSHARED_EXPORT Fw::Helpers::Locker
{
    Q_DISABLE_COPY(Locker)

    const Lockable& m_lockable;
    bool m_lock;

public:

    enum Mode
    {
        M_ReadLock,
        M_WriteLock,
        M_TryReadLock,
        M_TryWriteLock
    };

    explicit Locker(const Fw::Helpers::Lockable& lockable, const Mode& mode);
    ~Locker();

    inline bool isLock() const;
    inline void unlock();
};

bool Fw::Helpers::Locker::isLock() const
{
    return m_lock;
}

void Fw::Helpers::Locker::unlock()
{
    if(m_lock)
    {
        m_lockable.m_lock.unlock();
        m_lock = false;
        qDebug() << "unlock";
    }
}

#define FW_LOCK(obj, lockMode) for(Fw::Helpers::Locker l(obj, lockMode); l.isLock(); l.unlock())

#endif // FIREWORKS_HELPERS_LOCK_HPP
