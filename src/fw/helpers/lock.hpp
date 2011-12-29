#ifndef FIREWORKS_HELPERS_LOCK_HPP
#define FIREWORKS_HELPERS_LOCK_HPP

#include <QtCore/qreadwritelock.h>
#include <QtCore/qdebug.h>

#include "fw/defs.hpp"
#include "fw/helpers/defs.hpp"

class FIREWORKSSHARED_EXPORT Fw::Helpers::Lockable
{
    Q_DISABLE_COPY(Lockable)

protected:
    Lockable();
    virtual ~Lockable();

private:
    friend class Locker;
    mutable QReadWriteLock m_lock;
};

class FIREWORKSSHARED_EXPORT Fw::Helpers::Locker
{
    Q_DISABLE_COPY(Locker)

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

    void unlock();

private:
    const Lockable& m_lockable;
    bool m_lock;
};

bool Fw::Helpers::Locker::isLock() const
{
    return m_lock;
}

#define FW_TRY_READ_LOCK(obj) for(Fw::Helpers::Locker l(obj, Fw::Helpers::Locker::M_TryReadLock); l.isLock(); l.unlock())
#define FW_TRY_WRITE_LOCK(obj) for(Fw::Helpers::Locker l(obj, Fw::Helpers::Locker::M_TryWriteLock); l.isLock(); l.unlock())
#define FW_READ_LOCK(obj) for(Fw::Helpers::Locker l(obj, Fw::Helpers::Locker::M_ReadLock); l.isLock(); l.unlock())
#define FW_WRITE_LOCK(obj) for(Fw::Helpers::Locker l(obj, Fw::Helpers::Locker::M_WriteLock); l.isLock(); l.unlock())

#endif // FIREWORKS_HELPERS_LOCK_HPP
