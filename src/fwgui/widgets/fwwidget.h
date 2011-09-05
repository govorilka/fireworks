
#ifndef FIREWORKS_WIDGET_H
#define FIREWORKS_WIDGET_H

#include <QtCore/qlist.h>
#include <QtCore/qrect.h>

#include "fwtypography/fwfont.h"

#include "fwgui/primitives/fwprimitivegroup.h"

class FwScene;

class FwWidget;

class QPropertyAnimation;
class QEvent;
class QTimerEvent;

class FwShowEvent;
class FwHideEvent;
class FwKeyPressEvent;
class FwResizeEvent;

class FwResult;

class FIREWORKSSHARED_EXPORT FwWidget :  public QObject, public FwPrimitiveGroup
{
    Q_OBJECT
    typedef FwPrimitiveGroup BaseClass;

public:

    explicit FwWidget(const QByteArray& name, FwPrimitiveGroup* parent);
    virtual ~FwWidget();

    inline int visibleTime() const;
    void setVisibleTime(int msec);

    bool event(QEvent * event);

    inline bool isDarkBackgroundEnable() const;
    inline void setDarkBackground(bool value);

    void apply(FwMLObject *object);

    inline FwFont font() const;
    void setFont(const FwFont& font);

    inline QPropertyAnimation* showAnimation() const;
    void setShowAnimation(QPropertyAnimation* animation);

    inline QPropertyAnimation* hideAnimation() const;
    void setHideAnimation(QPropertyAnimation* animation);

    inline QRect dirtyRect(const QRect &rect) const;

    virtual bool setVisible(bool visible);

protected:
    void geometryChangedEvent(const QRect &oldRect, QRect &rect);

    virtual void keyPressEvent(FwKeyPressEvent* event);
    virtual void resizeEvent(FwResizeEvent* event);
    virtual void requestAcceptEvent(FwResult* result);

    virtual void timerEvent(QTimerEvent* event);
    virtual void showEvent(FwShowEvent* event);
    virtual void hideEvent(FwHideEvent* event);
    virtual void fontChangedEvent(const FwFont& font);

    virtual void visibleChangedEvent();

protected slots:
    virtual void showAnimationFinished();
    virtual void hideAnimationFinished();

private:

    void stopVisibleTimer();
    void startVisibleTimer();

    int m_visibleTime;
    int m_visibleTimerId;
    bool m_darkBackground;

    FwFont m_font;
    QPropertyAnimation* m_showAnimation;
    QPropertyAnimation* m_hideAnimation;

    QSize m_oldSize;
};

int FwWidget::visibleTime() const
{
    return m_visibleTime;
}

bool FwWidget::isDarkBackgroundEnable() const
{
    return m_darkBackground;
}

void FwWidget::setDarkBackground(bool value)
{
    m_darkBackground = value;
}

FwFont FwWidget::font() const
{
    return m_font;
}

QPropertyAnimation* FwWidget::showAnimation() const
{
    return m_showAnimation;
}

QPropertyAnimation* FwWidget::hideAnimation() const
{
    return m_hideAnimation;
}

QRect FwWidget::dirtyRect(const QRect& rect) const
{
    return geometry()->rect().intersect(rect);
}

#endif // FIREWORKS_WIDGET_H
