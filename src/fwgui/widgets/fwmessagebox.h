#ifndef FIREWORKS_MESSAGEBOX_H
#define FIREWORKS_MESSAGEBOX_H

#include "fwgui/widgets/fwwidget.h"

#include "fwgui/primitives/fwstringprimitive.h"

#include "fwutils/fwrequest.h"

class FwStringPrimitive;
class FwTextPrimitive;
class FwPixmapPrimitive;

class FwMessageBox;
class FwButtonsBox;

class FwImageLibrary;

class FIREWORKSSHARED_EXPORT FwMessageBoxButton : public FwPrimitiveGroup
{
    typedef FwPrimitiveGroup BaseClass;

public:
    FwMessageBoxButton(const FwRequestAnswer& answer, FwButtonsBox* parent);

    inline FwStringPrimitive* caption() const;
    inline FwPixmapPrimitive* icon() const;

private:
    FwStringPrimitive* m_caption;
    FwPixmapPrimitive* m_icon;
};

FwStringPrimitive* FwMessageBoxButton::caption() const
{
    return m_caption;
}

FwPixmapPrimitive* FwMessageBoxButton::icon() const
{
    return m_icon;
}

//////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT FwButtonsBox : public FwPrimitiveGroup
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

    FwButtonsBox(FwMessageBox* parent);

    inline int orientation() const;
    void setOrientation(Orientation value, bool needUpdatePos = true);

    static Orientation nameToOrientation(const QByteArray& name, bool*bOk = 0);
    static QByteArray orientationToName(Orientation value);

    inline int margin() const;
    inline void setMargin(int value);

    void apply(FwMLObject *object);

protected:

    void updateButtonsPos();
    void updateVerticalButtonsPos(int y);
    void updateHorizontalButtonsPos(int x);
    int totalWidth();
    int totalHeight();

private:
    QList<FwMessageBoxButton*> m_buttons;
    Orientation m_orientation;
    int m_margin;
    FwStringPrimitive* m_captionTemplate;
};

int FwButtonsBox::orientation() const
{
    return m_orientation;
}

int FwButtonsBox::margin() const
{
    return m_margin;
}
void FwButtonsBox::setMargin(int value)
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
    ~FwMessageBox();

    inline FwStringPrimitive* title() const;
    inline FwTextPrimitive* text() const;

    inline FwRequest request() const;
    void setRequest(const FwRequest& request);

    bool accept(int result);
    bool acceptKey(int key);

    void apply(FwMLObject *object);

    inline FwRectPrimitive* background() const;
    void setBackground(FwRectPrimitive* primitive);

protected:
    void keyPressEvent(FwKeyPressEvent *event);

private:
    FwRequest m_request;

    FwStringPrimitive* m_title;
    FwTextPrimitive* m_text;
    FwButtonsBox* m_buttonBox;

    FwRectPrimitive* m_background;
};

FwRequest FwMessageBox::request() const
{
    return m_request;
}

FwStringPrimitive* FwMessageBox::title() const
{
    return m_title;
}

FwTextPrimitive* FwMessageBox::text() const
{
    return m_text;
}

FwRectPrimitive* FwMessageBox::background() const
{
    return m_background;
}

#endif // FIREWORKS_MESSAGEBOX_H
