
#ifndef FIREWORKS_WIDGET_H
#define FIREWORKS_WIDGET_H

#include <QtCore/qlist.h>
#include <QtCore/qrect.h>

#include "fwtypography/fwfont.h"

#include "fwgui/fwgraphicsobject.h"

class FwScene;

class FwWidget;

class QPropertyAnimation;
class QEvent;
class QTimerEvent;

class FwShowEvent;
class FwHideEvent;

class FIREWORKSSHARED_EXPORT FwWidget : public FwGraphicsObject
{
    Q_OBJECT
    typedef FwGraphicsObject BaseClass;

public:

    explicit FwWidget(const QByteArray& name, FwPrimitiveGroup* parent);
    virtual ~FwWidget();

    inline int visibleTime() const;
    inline void setVisibleTime(int msec);

    void stopVisibleTimer();
    void startVisibleTimer();

    bool event(QEvent * event);

    inline bool isDarkBackgroundEnable() const;
    inline void setDarkBackground(bool value);

    void apply(FwMLObject *object);

protected:
    virtual void timerEvent(QTimerEvent* event);
    virtual void showEvent(FwShowEvent* event);
    virtual void hideEvent(FwHideEvent* event);
    virtual void fontChangedEvent(const FwFont& font);

    virtual void visibleChangedEvent();

    void showAnimationFinished();
    void hideAnimationFinished();

private:
    int m_visibleTime;
    int m_visibleTimerId;
    bool m_darkBackground;
};

int FwWidget::visibleTime() const
{
    return m_visibleTime;
}

void FwWidget::setVisibleTime(int msec)
{
    if(m_visibleTime != msec)
    {
        (m_visibleTime = msec) ? startVisibleTimer() : stopVisibleTimer();
    }
}

bool FwWidget::isDarkBackgroundEnable() const
{
    return m_darkBackground;
}

void FwWidget::setDarkBackground(bool value)
{
    m_darkBackground = value;
}

#endif // FIREWORKS_WIDGET_H
