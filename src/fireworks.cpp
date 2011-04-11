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

Fw::Orientation Fw::nameToOrientation(const QByteArray& name, bool* bOk)
{
    QByteArray lowerName = name.toLower();
    if(lowerName == "horizontal")
    {
        if(bOk)
        {
            (*bOk) = true;
        }
        return O_Horizontal;
    }
    else if(lowerName == "vertical")
    {
        if(bOk)
        {
            (*bOk) = true;
        }
        return O_Vertical;
    }
    else if(lowerName == "diagonal")
    {
        if(bOk)
        {
            (*bOk) = true;
        }
        return O_Diagonal;
    }

    if(bOk)
    {
        (*bOk) = false;
    }
    return O_Horizontal;
}

Fw::HorizontalPosition Fw::nameToHorizontalPosition(const QByteArray& name, bool *bOk)
{
    if(bOk)
    {
        (*bOk) = false;
    }
    return HP_Left;
}

Fw::VerticalPosition Fw::nameToVerticalPosition(const QByteArray& name, bool *bOk)
{
    if(bOk)
    {
        (*bOk) = false;
    }
    return VP_Top;
}
