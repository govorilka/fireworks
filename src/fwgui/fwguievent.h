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
    FwKeyPressEvent(int key, int autoRepeatCount);

    inline int key() const;
    inline void setKey(int newKey);

    inline bool isAutoRepeat() const;

    inline QString text() const;
    inline void setText(const QString& text);

    inline int count() const;

private:
    int m_key;
    int m_autoRepeatCount;
    QString m_text;
};

int FwKeyPressEvent::key() const
{
    return m_key;
}

void FwKeyPressEvent::setKey(int newKey)
{
    m_key = newKey;
}

bool FwKeyPressEvent::isAutoRepeat() const
{
    return m_autoRepeatCount > 0;
}

QString FwKeyPressEvent::text() const
{
    return m_text;
}

void FwKeyPressEvent::setText(const QString& text)
{
    m_text = text;
}

int FwKeyPressEvent::count() const
{
    return m_autoRepeatCount + 1;
}

#endif // FIREWORKS_GUIEVENT_H
