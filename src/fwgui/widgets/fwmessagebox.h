#ifndef FIREWORKS_MESSAGEBOX_H
#define FIREWORKS_MESSAGEBOX_H

#include "fwgui/widgets/fwwidget.h"

#include "fwgui/primitives/fwstringprimitive.h"

#include "fwutils/fwrequest.h"

class FwStringPrimitive;
class FwPixmapPrimitive;

class FwMessageBox;
class FwMessageBoxButtons;

class FIREWORKSSHARED_EXPORT FwMessageBoxButton : public FwPrimitiveGroup
{
    typedef FwPrimitiveGroup BaseClass;

public:
    FwMessageBoxButton(const FwRequestAnswer& answer, FwMessageBoxButtons* parent);

    inline FwStringPrimitive* caption() const;
    inline FwPixmapPrimitive* icon() const;

    inline int result() const;
    inline void setResult(int value);

private:
    FwStringPrimitive* m_caption;
    FwPixmapPrimitive* m_icon;
    int m_result;
};

FwStringPrimitive* FwMessageBoxButton::caption() const
{
    return m_caption;
}

FwPixmapPrimitive* FwMessageBoxButton::icon() const
{
    return m_icon;
}

int FwMessageBoxButton::result() const
{
    return m_result;
}

void FwMessageBoxButton::setResult(int value)
{
    m_result = value;
}

//////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT FwMessageBoxButtons : public FwPrimitiveGroup
{
typedef FwPrimitiveGroup BaseClass;

public:

    enum Orientation
    {
        O_Invalid,
        O_Top,
        O_Middle,
        O_Bottom,
        O_Left,
        O_Center,
        O_Right
    };

    FwMessageBoxButtons(const QList<FwRequestAnswer> answers, FwMessageBox* parent);

    inline int orientation() const;
    inline void setOrientation(Orientation value);

    static Orientation nameToOrientation(const QByteArray& name, bool*bOk = 0);
    static QByteArray orientationToName(Orientation value);

    inline int margin() const;
    inline void setMargin(int value);

    void apply(FwMLObject *object);

private:
    QList<FwMessageBoxButton*> m_buttons;
    Orientation m_orientation;
    int m_margin;
};

int FwMessageBoxButtons::orientation() const
{
    return m_orientation;
}

void FwMessageBoxButtons::setOrientation(Orientation value)
{
    m_orientation = value;
}

int FwMessageBoxButtons::margin() const
{
    return m_margin;
}
void FwMessageBoxButtons::setMargin(int value)
{
    m_margin = value;
}

//////////////////////////////////////////////////////////////


class FIREWORKSSHARED_EXPORT FwMessageBox : public FwWidget
{
    Q_OBJECT
    typedef FwWidget BaseClass;

public:
    explicit FwMessageBox(const QByteArray& name, FwPrimitiveGroup* parent);

    inline FwStringPrimitive* caption() const;

    void setRequest(const FwRequest& request);

    bool accept(int result);
    bool acceptKey(int key);

    inline FwStringPrimitive* messageText() const;
    inline void setMessageText(const QString& value);

protected:
    void keyPressEvent(FwKeyPressEvent *event);

private:
    FwStringPrimitive* m_caption;
    FwRequest m_request;
    FwStringPrimitive* m_messageText;
    FwMessageBoxButtons* m_messageboxbuttons;
};

FwStringPrimitive* FwMessageBox::caption() const
{
    return m_caption;
}

FwStringPrimitive* FwMessageBox::messageText() const
{
    return m_messageText;
}

#endif // FIREWORKS_MESSAGEBOX_H
