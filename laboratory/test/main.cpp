#include <QtCore/QCoreApplication>
#include <QtCore/qdebug.h>
#include <QtCore/qbytearray.h>

#include "fw/helpers/lock.hpp"

class Lockable : public Fw::Helpers::Lockable
{
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    try
    {
        Lockable obj;

        FW_LOCK(obj, Fw::Helpers::Locker::M_ReadLock)
        {
            qDebug() << "Action";
        }
        FW_LOCK(obj, Fw::Helpers::Locker::M_WriteLock)
        {
            qDebug() << "Action";
        }
        FW_LOCK(obj, Fw::Helpers::Locker::M_TryReadLock)
        {
            throw 10;
            qDebug() << "Action";
        }
        FW_LOCK(obj, Fw::Helpers::Locker::M_TryWriteLock)
        {
            qDebug() << "Action";
        }
    }
    catch(const QByteArray& s)
    {
        qDebug() << s;
    }
    catch(...)
    {
        qDebug() << "FUCK";
    }

    return 0;
}
