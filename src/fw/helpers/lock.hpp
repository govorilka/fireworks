#ifndef FIREWORKS_HELPERS_LOCK_HPP
#define FIREWORKS_HELPERS_LOCK_HPP

#include <QtCore/qreadwritelock.h>

#include "fw/defs.hpp"
#include "fw/helpers/defs.hpp"


class FIREWORKSSHARED_EXPORT Fw::Helpers::Lockable
{
    friend class Locker;
    QReadWriteLock m_lock;

protected:
    Lockable();
    virtual ~Lockable();
};

class FIREWORKSSHARED_EXPORT Fw::Helpers::Locker
{
    Q_DISABLE_COPY(Locker);

    Lockable& m_lockable;
    bool m_lock;

public:

    enum Mode
    {
        M_ReadLock,
        M_WriteLock,
        M_TryReadLock,
        M_TryWriteLock
    };

    explicit Locker(Lockable& lockable, const Mode& mode);
    ~Locker();

    inline bool isLock() const;
};

bool Fw::Helpers::Locker::isLock() const
{
    return m_lock;
}

#endif // FIREWORKS_HELPERS_LOCK_HPP
