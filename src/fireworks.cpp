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
        (*bOk) = true;
    }
 QByteArray temp = name.trimmed();
 temp = temp.toLower();
 if(temp == "unchanged")
 {
    return HP_Unchanged;
 }
 else if(temp == "beforeleft")
 {
    return HP_BeforeLeft;
 }
 else if(temp == "left")
 {
    return HP_Left;
 }
else if(temp == "center")
{
    return HP_Center;
}
else if(temp == "centerdock")
{
    return HP_CenterDock;
}
else if(temp == "right")
{
    return HP_Right;
}
else if(temp == "afterright")
{
    return HP_AfterRight;
}
else
{
    (*bOk) = false;
    return HP_Left;
}
}
Fw::VerticalPosition Fw::nameToVerticalPosition(const QByteArray& name, bool *bOk)
{
    if(bOk)
    {
        (*bOk) = true;
    }

    QByteArray temp = name.trimmed().toLower();
    if(temp == "unchanged")
    {
        return VP_Unchanged;
    }
    else if(temp == "beforetop")
    {
        return VP_BeforeTop;
    }
    else if(temp == "top")
    {
        return VP_Top;
    }
    else if(temp == "middle")
    {
        return VP_Middle;
    }
    else if(temp == "middledock")
    {
        return VP_MiddleDock;
    }
    else if(temp == "bottom")
    {
        return VP_Bottom;
    }
    else if(temp == "afterbottom")
    {
        return VP_AfterBottom;
    }
    else
    {
        (*bOk) = false;
        return VP_Top;
    }

}

QByteArray Fw::verticalPositionToname(VerticalPosition position)
{
    switch(position)
    {
    case HP_Unchanged:
        return "unchanged";
    case VP_BeforeTop:
        return "beforetop";
    case VP_Top:
        return "top";
    case VP_Middle:
        return "middle";
    case VP_MiddleDock:
        return "middledock";
    case VP_Bottom:
        return "bottom";
    case VP_AfterBottom:
        return "afterbottom";
    }
}

QByteArray Fw::horizontalPositionToName(HorizontalPosition position)
{

    switch(position)
    {
    case HP_Unchanged:
        return "unchanged";
    case HP_BeforeLeft:
        return "beforeleft";
    case HP_Left:
        return "left";
    case HP_Center:
        return "center";
    case HP_CenterDock:
        return "centerdock";
    case HP_Right:
        return "right";
    case HP_AfterRight:
        return "afterright";
    }
}
