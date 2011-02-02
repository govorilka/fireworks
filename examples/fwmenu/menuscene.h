#ifndef MENUSCENE_H
#define MENUSCENE_H

#include "fwgui/fwscene.h"

class FwSlidingFramePrimitive;
class FwStringPrimitive;

class MenuScene : public FwScene
{
    typedef FwScene BaseClass;

public:
    MenuScene(int id, FwGraphicsView* view);

protected:
    void keyPressEvent(FwKeyPressEvent *event);

private:
    FwSlidingFramePrimitive* m_slidingFrame;

    FwStringPrimitive* m_item1;
    FwStringPrimitive* m_item2;
    FwStringPrimitive* m_item3;
    FwStringPrimitive* m_item4;
    FwStringPrimitive* m_item5;
};

#endif // MENUSCENE_H
