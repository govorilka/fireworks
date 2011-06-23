#ifndef PLAYERSCENE5_H
#define PLAYERSCENE5_H

#include "fwgui/fwscene.h"

class FwCheckableItemView;

class PlayerScene5 : public FwScene
{
    Q_OBJECT
    typedef FwScene BaseClass;
public:
    PlayerScene5(int id, FwGraphicsView* view);

    void apply(FwMLObject *object);

protected:
    void keyPressEvent(FwKeyPressEvent *event);
    void showEvent(FwSceneShowEvent *event);

    void invalidateChildrenRect();

    void requestAcceptEvent(FwResult* result);

private:
    FwCheckableItemView* m_itemView;
};

#endif // PLAYERSCENE5_H
