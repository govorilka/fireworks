#ifndef FIREWORKS_GRAPHICSOBJECT_H
#define FIREWORKS_GRAPHICSOBJECT_H

#include <QtCore/qobject.h>

#include "fwgui/primitives/fwprimitivegroup.h"

#include "fwtypography/fwfont.h"

class QResizeEvent;
class QPropertyAnimation;

class FwResizeEvent;
class FwKeyPressEvent;

class FwObjectGraphicsItem;

class FwGraphicsObject : public QObject, public FwPrimitiveGroup
{
    Q_OBJECT
    typedef FwPrimitiveGroup BaseClass;

public:
    friend class FwScene;
    friend class FwWidget;
    friend class FwPrimitive;

    explicit FwGraphicsObject(const QByteArray& name, FwPrimitiveGroup* parent);
    virtual ~FwGraphicsObject();

    inline FwFont font() const;
    void setFont(const FwFont& font);

    inline QPropertyAnimation* showAnimation() const;
    void setShowAnimation(QPropertyAnimation* animation);

    inline QPropertyAnimation* hideAnimation() const;
    void setHideAnimation(QPropertyAnimation* animation);

    bool event(QEvent *e);

    FwGraphicsObject* object() const;

protected:
    void geometryChangedEvent(const QRect &oldRect, QRect &rect);

    virtual void keyPressEvent(FwKeyPressEvent* event);
    virtual void resizeEvent(FwResizeEvent* event);

protected slots:
    virtual void showAnimationFinished() = 0;
    virtual void hideAnimationFinished() = 0;

private:
    FwFont m_font;
    QPropertyAnimation* m_showAnimation;
    QPropertyAnimation* m_hideAnimation;

    QSize m_oldSize;
};

#include "fwgui/fwgraphicsobject_inl.h"

#endif // FIREWORKS_GRAPHICSOBJECT_H
