#ifndef FIREWORKS_H
#define FIREWORKS_H

#include <QtCore/qlist.h>
#include <QtCore/qdir.h>
#include <QtCore/qbytearray.h>

#include "fw/exception.hpp"

namespace Fw
{
    enum HorizontalPosition
    {
        HP_Unchanged,
        HP_BeforeLeft,
        HP_Left,
        HP_Center,
        HP_CenterDock,
        HP_Right,
        HP_AfterRight
    };

    enum VerticalPosition
    {
        VP_Unchanged,
        VP_BeforeTop,
        VP_Top,
        VP_Middle,
        VP_MiddleDock,
        VP_Bottom,
        VP_AfterBottom
    };

    enum SchedulerTaskStatus
    {
        TS_Unknow,
        TS_Start,
        TS_Stop,
        TS_Pause,
        TS_UserRequest
    };

    enum BufferMode
    {
        BM_NoBuffer = 0,
        BM_SystemDefault,
        BM_VideoOnly,
        BM_SystemOnly
    };

    enum Orientation
    {
        O_Horizontal,
        O_Vertical,
        O_Diagonal
    };

    FIREWORKSSHARED_EXPORT Orientation nameToOrientation(const QByteArray& name, bool* bOk = 0);

    enum GuiEventType
    {
        E_Unknow,
        E_KeyPress,
        E_Resize,
        E_Show,
        E_Hide,
        E_SceneShow,
        E_SceneHide
    };

    enum Navigation
    {
        Go_Next,
        Go_Previous,
        Go_Back,
        Go_First,
        Go_Last
    };

    enum ModalResult
    {
        MR_Ok,
        MR_Cancel
    };

    FIREWORKSSHARED_EXPORT int go(Navigation nav, int size, int current, int previous = -1);

    template <class T> T* goPtr(const QList<T*> list, Navigation nav, T* current, T* previous = 0)
    {
        int result = go(nav,
                        list.size(),
                        list.indexOf(current),
                        previous ? list.indexOf(previous) : -1);

        if(result != -1)
        {
            return list.at(result);
        }

        return 0;
    }

    FIREWORKSSHARED_EXPORT QString cacheDirectory(const QString& cacheName);
    FIREWORKSSHARED_EXPORT QString tempDirectory();
    FIREWORKSSHARED_EXPORT QString dataDirectory();

    FIREWORKSSHARED_EXPORT bool changedDirectory(QDir* dir, const QString& dirName);

    FIREWORKSSHARED_EXPORT HorizontalPosition nameToHorizontalPosition(const QByteArray& name, bool *bOk);
    FIREWORKSSHARED_EXPORT QByteArray horizontalPositionToName(HorizontalPosition position);

    FIREWORKSSHARED_EXPORT VerticalPosition nameToVerticalPosition(const QByteArray& name, bool *bOk);
    FIREWORKSSHARED_EXPORT QByteArray verticalPositionToname(VerticalPosition position);

    const char constantTrue[] = "true";
    const char constantFalse[] = "false";

    FIREWORKSSHARED_EXPORT bool nameToBool(const QByteArray&, bool* bOk);
    FIREWORKSSHARED_EXPORT QByteArray boolToName(bool value);
}

#endif // FIREWORKS_H
