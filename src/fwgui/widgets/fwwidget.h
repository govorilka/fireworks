#ifndef FIREWORKS_WIDGET_H
#define FIREWORKS_WIDGET_H

#include <QtCore/qlist.h>
#include <QtCore/qrect.h>

#include <QtGui/qregion.h>

#include "fwtypography/fwfont.h"

#include "fwgui/fwgraphicsobject.h"

class FwScene;

class Primitive;
class PrimitiveGroup;
class RootPrimitive;
class FwWidget;

class QPropertyAnimation;
class QEvent;
class QTimerEvent;
class QShowEvent;
class QHideEvent;
class QResizeEvent;

class FwWidget : public FwGraphicsObject
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

protected:
    virtual void timerEvent(QTimerEvent* event);
    virtual void showEvent(QShowEvent* event);
    virtual void hideEvent(QHideEvent* event);
    virtual void resizeEvent(QResizeEvent* event);
    virtual void fontChangedEvent(const FwFont& font);

    virtual void visibleChangedEvent();

    //void showEventProcessed(QShowEvent* event);
    //void hideEventProcessed(QHideEvent* event);

    void showAnimationFinished();
    void hideAnimationFinished();

private:
    int m_visibleTime;
    int m_visibleTimerId;
};

#include "fwgui/widgets/fwwidget_inl.h"

#endif // FIREWORKS_WIDGET_H
