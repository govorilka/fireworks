#ifndef FIREWORKS_SCENE_H
#define FIREWORKS_SCENE_H

#include <QtCore/qlist.h>
#include <QtCore/qcoreevent.h>
#include <QtCore/qpointer.h>

#include "fwtypography/fwfont.h"

#include "fwgui/primitives/fwprimitivegroup.h"

class FwSceneGraphicsItem;
class FwWidget;
class FwGraphicsView;
class FwMessageBox;

class FwRequest;
class FwResult;

class FwSceneShowEvent;
class FwSceneHideEvent;
class FwResizeEvent;
class FwKeyPressEvent;

class FIREWORKSSHARED_EXPORT FwScene : public QObject, public FwPrimitiveGroup
{
    Q_OBJECT
    typedef FwPrimitiveGroup BaseClass;

public:
    friend class FwWidget;
    friend class FwGraphicsView;
    friend class FwPrimitive;
    friend class FwPrimitiveGroup;
    friend class FwMessageBox;

    explicit FwScene(const QByteArray& name, FwGraphicsView* view);
    virtual ~FwScene();

    bool event(QEvent* event);

    FwFont font(const FwFontDescription& description);

    inline FwGraphicsView* view() const;

    bool isActive() const;

    inline FwMessageBox* messagebox() const;

    inline FwMLObject* messageBoxTemplate() const;
    void setMessageBoxTemplate(FwMLObject* temp);

    void apply(FwMLObject *object);

    inline bool isMessageBoxAllow() const;
    inline void setMessageBoxAllow(bool value);

    inline FwRectPrimitive* darkBackground() const;

    virtual void cleanup();

    inline QPointer<FwScene> lastScene() const;

protected:
    void showEventProcessed(FwSceneShowEvent* e);
    void hideEventProcessed(FwSceneHideEvent* e);

    virtual void showEvent(FwSceneShowEvent* event);
    virtual void hideEvent(FwSceneHideEvent* event);
    virtual void resizeEvent(FwResizeEvent* event);

    virtual void keyPressEvent(FwKeyPressEvent* event);

    void showAnimationFinished();
    void hideAnimationFinished();

    virtual FwMessageBox* createMessageBox(FwMLObject* messageBoxTemplate);
    bool showMessageBox(const FwRequest& request);

    void invalidate();

    bool keyEventProccessed(FwKeyPressEvent* event);

private:
    FwGraphicsView* m_view;

    QList<FwWidget*> m_widgets;

    FwMessageBox* m_messageBox;
    FwMLObject* m_messageBoxTemplate;

    bool m_messageBoxAllow;

    FwRectPrimitive* m_darkBackground;
};

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

bool FwScene::isMessageBoxAllow() const
{
    return m_messageBoxAllow;
}

void FwScene::setMessageBoxAllow(bool value)
{
    m_messageBoxAllow = value;
}

FwRectPrimitive* FwScene::darkBackground() const
{
    return m_darkBackground;
}

#endif // FWSCENE_H
