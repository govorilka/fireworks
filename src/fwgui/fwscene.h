#ifndef FIREWORKS_SCENE_H
#define FIREWORKS_SCENE_H

#include <QtCore/qlist.h>
#include <QtCore/qcoreevent.h>

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
    friend class FwPrimitive;

    explicit FwScene(int id, FwGraphicsView* view);
    virtual ~FwScene();

    inline int id() const;

    bool event(QEvent* event);

    FwFont font(const FwFontDescription& description);

    inline FwGraphicsView* view() const;

    bool isActive() const;

protected:
    void showEventProcessed(FwSceneShowEvent* e);
    void hideEventProcessed(FwSceneHideEvent* e);

    virtual void showEvent(FwSceneShowEvent* event);
    virtual void hideEvent(FwSceneHideEvent* event);

    void showAnimationFinished();
    void hideAnimationFinished();

private:
    FwGraphicsView* m_view;
    int m_id;

    QList<FwWidget*> m_widgets;
};

int FwScene::id() const
{
    return m_id;
}

FwGraphicsView* FwScene::view() const
{
    return m_view;
}

#endif // FWSCENE_H
