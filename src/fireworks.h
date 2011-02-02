#ifndef FIREWORKS_H
#define FIREWORKS_H

#include <QtCore/qglobal.h>
#include <QtCore/qlist.h>

#if defined(FIREWORKS_LIBRARY)
#  define FIREWORKSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FIREWORKSSHARED_EXPORT Q_DECL_IMPORT
#endif

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

    enum BufferMode
    {
        BM_NoBuffer = 0,
        BM_SystemDefault,
        BM_VideoOnly,
        BM_SystemOnly,
    };

    enum Orientation
    {
        O_Horizontal,
        O_Vertical,
        O_Diagonal
    };

    enum GuiEventType
    {
        E_Unknow,
        E_KeyPress,
    };

    enum Navigation
    {
        Go_Next,
        Go_Previous,
        Go_Back,
        Go_First,
        Go_Last,
    };

    int go(Navigation nav, int size, int current, int previous);

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

}

#endif // FIREWORKS_H
