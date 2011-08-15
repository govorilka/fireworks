#ifndef FIREWORKS_GUIEVENT_H
#define FIREWORKS_GUIEVENT_H

#include <QtCore/qcoreevent.h>
#include <QtCore/qsize.h>

#include "fireworks.h"

class FwScene;

class FIREWORKSSHARED_EXPORT FwGuiEvent : public QEvent
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

class FIREWORKSSHARED_EXPORT FwKey
{
public:
    FwKey();
    FwKey(quint32 qtKey, qint8 number = -1, char ansciiChar = 0);

    inline bool isNull() const;

    inline quint32 qtKey() const;
    inline qint8 number() const;
    inline char ansciiChar() const;

    inline bool isNumder() const;

private:
    quint32 m_qtKey;
    qint8 m_number;
    char m_ansciiChar;
};

bool FwKey::isNull() const
{
    return m_qtKey == 0;
}

quint32 FwKey::qtKey() const
{
    return m_qtKey;
}

qint8 FwKey::number() const
{
    return m_number;
}

char FwKey::ansciiChar() const
{
    return m_ansciiChar;
}

bool FwKey::isNumder() const
{
    return m_number != -1;
}

//////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT FwKeyPressEvent : public FwGuiEventTmpl<Fw::E_KeyPress>
{
    typedef FwGuiEventTmpl<Fw::E_KeyPress> BaseClass;
public:
    FwKeyPressEvent();
    FwKeyPressEvent(const FwKey& key, int autoRepeatCount);

    inline FwKey key() const;
    inline void setKey(const FwKey& key);

    inline bool isAutoRepeat() const;

    inline QString text() const;
    inline void setText(const QString& text);

    inline int count() const;

private:
    FwKey m_key;
    int m_autoRepeatCount;
    QString m_text;
};

FwKey FwKeyPressEvent::key() const
{
    return m_key;
}

void FwKeyPressEvent::setKey(const FwKey& key)
{
    m_key = key;
}

bool FwKeyPressEvent::isAutoRepeat() const
{
    return m_autoRepeatCount > 1;
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
    return m_autoRepeatCount;
}

//////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT FwResizeEvent : public FwGuiEventTmpl<Fw::E_Resize>
{
    typedef FwGuiEventTmpl<Fw::E_Resize> BaseClass;

public:
    FwResizeEvent();
    FwResizeEvent(const QSize& olsSize, const QSize& newSize);

    inline QSize oldSize() const;
    inline QSize newSize() const;

private:
    QSize m_oldSize;
    QSize m_newSize;
};

QSize FwResizeEvent::oldSize() const
{
    return m_oldSize;
}

QSize FwResizeEvent::newSize() const
{
    return m_newSize;
}

//////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT FwShowEvent : public FwGuiEventTmpl<Fw::E_Show>
{
    typedef FwGuiEventTmpl<Fw::E_Show> BaseClass;

public:
    FwShowEvent(bool visibleOnScreen);

    inline bool visibleOnScreen() const;

private:
    bool m_visibleOnScreen;
};

bool FwShowEvent::visibleOnScreen() const
{
    return m_visibleOnScreen;
}

//////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT FwHideEvent : public FwGuiEventTmpl<Fw::E_Hide>
{
    typedef FwGuiEventTmpl<Fw::E_Hide> BaseClass;

public:
    FwHideEvent();
};

//////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT FwSceneShowEvent : public FwGuiEventTmpl<Fw::E_SceneShow>
{
    typedef FwGuiEventTmpl<Fw::E_SceneShow> BaseClass;

public:
    FwSceneShowEvent(FwScene* previous);

    inline FwScene* previous() const;

private:
    FwScene* m_previous;
};

FwScene* FwSceneShowEvent::previous() const
{
    return m_previous;
}

//////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT FwSceneHideEvent : public FwGuiEventTmpl<Fw::E_SceneHide>
{
    typedef FwGuiEventTmpl<Fw::E_SceneHide> BaseClass;

public:
    FwSceneHideEvent(FwScene* next);

    inline FwScene* next() const;

private:
    FwScene* m_next;
};

FwScene* FwSceneHideEvent::next() const
{
    return m_next;
}

#endif // FIREWORKS_GUIEVENT_H
