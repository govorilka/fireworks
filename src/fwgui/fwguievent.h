#ifndef FIREWORKS_GUIEVENT_H
#define FIREWORKS_GUIEVENT_H

#include <QtCore/qcoreevent.h>

#include "fireworks.h"

class FwGuiEvent : public QEvent
{
    typedef QEvent BaseClass;

public:

    FwGuiEvent();

    virtual Fw::GuiEventType eventType() const = 0;

    static int qtTypeID();
};

//////////////////////////////////////////////////////////////////////////////

template <Fw::GuiEventType _type> class FwGuiEventTmpl : public FwGuiEvent
{
    typedef FwGuiEvent BaseClass;
public:

    static const Fw::GuiEventType fwEventType = _type;

    Fw::GuiEventType eventType() const
    {
        return fwEventType;
    }
};

//////////////////////////////////////////////////////////////////////////////

class FwKeyPressEvent : public FwGuiEventTmpl<Fw::E_KeyPress>
{
    typedef FwGuiEventTmpl<Fw::E_KeyPress> BaseClass;
public:
    FwKeyPressEvent();
    FwKeyPressEvent(int key, bool autoRepeat);

    inline int key() const;
    inline bool isAutoRepeat() const;

private:
    int m_key;
    bool m_autoRepeat;
};

int FwKeyPressEvent::key() const
{
    return m_key;
}

bool FwKeyPressEvent::isAutoRepeat() const
{
    return m_autoRepeat;
}

#endif // FIREWORKS_GUIEVENT_H
