#ifndef PLAYERSCENE5_H
#define PLAYERSCENE5_H

#include "fwgui/fwscene.h"

class FwCheckableItemView;
class FwDigitInputWidget;

class PlayerScene5 : public FwScene
{
    Q_OBJECT
    typedef FwScene BaseClass;
public:
    PlayerScene5(FwGraphicsView* view);

    void apply(FwMLObject *object);

protected:
    void keyPressEvent(FwKeyPressEvent *event);
    void showEvent(FwSceneShowEvent *event);

    void invalidateChildrenRect();

    void requestAcceptEvent(FwResult* result);

private:
    FwCheckableItemView* m_itemView;
    FwDigitInputWidget* m_digitInput;

private slots:
    void valueChanged(int value);
};

#endif // PLAYERSCENE5_H
