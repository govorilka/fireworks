#include "fireworks.h"

int Fw::go(Navigation nav, int size, int current, int previous)
{
    if(size > 0)
    {
        switch(nav)
        {
        case Fw::Go_Next:
            return (current == size - 1) ? 0 : current + 1;

        case Fw::Go_Previous:
            return (current == 0) ? size - 1 : current - 1;

        case Fw::Go_Back:
            if(previous >= 0 && previous < size)
            {
                return previous;
            }
            break;

        case Fw::Go_First:
            return 0;

        case Fw::Go_Last:
            return size -1;

        default:
            break;
        }
    }
    return -1;
}

QString Fw::cacheDirectory(const QString& cacheName)
{
    QDir dir = QDir::home();
    if(changedDirectory(&dir, ".mw") &&
       changedDirectory(&dir, "cache") &&
       changedDirectory(&dir, cacheName))
    {
        return dir.canonicalPath();
    }
    return "";
}

QString Fw::tempDirectory()
{
    QDir dir = QDir::home();
    if(changedDirectory(&dir, ".mw") &&
       changedDirectory(&dir, "temp"))
    {
        return dir.canonicalPath();
    }
    return "";
}

QString  Fw::dataDirectory()
{
    QDir dir = QDir::home();
    if(changedDirectory(&dir, ".mw"))
    {
        return dir.canonicalPath();
    }
    return "";
}

bool Fw::changedDirectory(QDir* dir, const QString& dirName)
{
    if(!dir->cd(dirName))
    {
        if(!dir->mkdir(dirName))
        {
            QString error = QString("Cannot create %1 directory!").arg(dirName);
            qFatal(error.toUtf8().data());
            return false;
        }
        dir->cd(dirName);
    }
    return true;
}
