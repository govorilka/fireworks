#ifndef MENUSCENE_H
#define MENUSCENE_H

#include "fwgui/fwscene.h"

class FwSlidingFramePrimitive;
class FwStringPrimitive;
class FwItemView;

class MenuScene : public FwScene
{
    Q_OBJECT
    typedef FwScene BaseClass;

public:
    MenuScene(int id, FwGraphicsView* view);

protected:
    void keyPressEvent(FwKeyPressEvent *event);

private:
    FwItemView* m_itemView;

private slots:
    void testCase1();
    void testCase2();
};

#endif // MENUSCENE_H
