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
class FwMessageBox;
class FwRequest;

class QResizeEvent;
class QKeyEvent;

class FIREWORKSSHARED_EXPORT FwScene : public FwGraphicsObject
{
    Q_OBJECT
    typedef FwGraphicsObject BaseClass;

public:
    friend class FwWidget;
    friend class FwGraphicsView;
    friend class FwPrimitive;
    friend class FwPrimitiveGroup;
    friend class FwMessageBox;

    explicit FwScene(int id, FwGraphicsView* view);
    virtual ~FwScene();

    inline int id() const;

    bool event(QEvent* event);

    FwFont font(const FwFontDescription& description);

    inline FwGraphicsView* view() const;

    bool isActive() const;

    inline FwMessageBox* messagebox() const;

    inline FwMLObject* messageBoxTemplate() const;
    void setMessageBoxTemplate(FwMLObject* temp);

    void apply(FwMLObject *object);

    void postRequest(const FwRequest& request);

    FwMessageBox* createMessageBox(FwMLObject* messageBoxTemplate);

protected:
    void showEventProcessed(FwSceneShowEvent* e);
    void hideEventProcessed(FwSceneHideEvent* e);

    virtual void showEvent(FwSceneShowEvent* event);
    virtual void hideEvent(FwSceneHideEvent* event);

    void showAnimationFinished();
    void hideAnimationFinished();

    void invalidate();

    bool keyEventProccessed(FwKeyPressEvent* event);

private:
    FwGraphicsView* m_view;
    int m_id;

    QList<FwWidget*> m_widgets;

    FwMessageBox* m_messageBox;
    FwMLObject* m_messageBoxTemplate;
};

int FwScene::id() const
{
    return m_id;
}

FwGraphicsView* FwScene::view() const
{
    return m_view;
}

FwMessageBox* FwScene::messagebox() const
{
    return m_messageBox;
}

FwMLObject* FwScene::messageBoxTemplate() const
{
    return m_messageBoxTemplate;
}

#endif // FWSCENE_H
