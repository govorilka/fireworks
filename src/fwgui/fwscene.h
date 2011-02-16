#ifndef FIREWORKS_SCENE_H
#define FIREWORKS_SCENE_H

#include <QtCore/qlist.h>
#include <QtCore/qcoreevent.h>

#include <QtGui/qregion.h>

#include "fwgui/fwgraphicsobject.h"

#include "fwtypography/fwfont.h"

class FwSceneShowEvent;
class FwSceneHideEvent;
class FwSceneGraphicsItem;
class FwWidget;
class FwGraphicsView;

class QResizeEvent;
class QKeyEvent;

class FwScene : public FwGraphicsObject
{
    Q_OBJECT
    typedef FwGraphicsObject BaseClass;

public:
    friend class FwWidget;
    friend class FwGraphicsView;

    explicit FwScene(int id, FwGraphicsView* view);
    virtual ~FwScene();

    inline int id() const;

    bool event(QEvent* event);

    FwFont font(const FwFontDescription& description);

    inline FwGraphicsView* view() const;

    void updateCanvas(const QRect& rect);

protected:
    void showEventProcessed(FwSceneShowEvent* e);
    void hideEventProcessed(FwSceneHideEvent* e);

    virtual void showEvent(FwSceneShowEvent* event);
    virtual void hideEvent(FwSceneHideEvent* event);

    void showAnimationFinished();
    void hideAnimationFinished();

    void paint(FwPainter *painter, const QRect &clipRect);

private:
    FwGraphicsView* m_view;
    int m_id;

    QList<FwWidget*> m_widgets;
};

////////////////////////////////////////////////////////////////////////////////

class FwSceneShowEvent : public QEvent
{
    typedef QEvent BaseClass;

public:
    FwSceneShowEvent(FwScene* next);

    static int typeID();

    inline FwScene* previous() const;

private:
    FwScene* m_previous;
};

////////////////////////////////////////////////////////////////////////////////

class FwSceneHideEvent : public QEvent
{
    typedef QEvent BaseClass;
public:
    FwSceneHideEvent(FwScene* next);

    static int typeID();

    inline FwScene* next() const;

private:
    FwScene* m_next;
};

#include "fwgui/fwscene_inl.h"

#endif // FWSCENE_H
