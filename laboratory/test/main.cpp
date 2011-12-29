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

        FW_READ_LOCK(obj)
        {
            qDebug() << "Action";
        }
        FW_WRITE_LOCK(obj)
        {
            qDebug() << "Action";
        }
        FW_TRY_READ_LOCK(obj)
        {
            qDebug() << "Action";
        }
        FW_TRY_WRITE_LOCK(obj)
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
