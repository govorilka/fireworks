#ifndef FIREWORKS_EXAMPLE_PLAYERSCENE7_H
#define FIREWORKS_EXAMPLE_PLAYERSCENE7_H

#include "fwgui/fwscene.h"

class FwPixmapBar;

class PlayerScene7 : public FwScene
{
    Q_OBJECT
    typedef FwScene BaseClass;

public:
    PlayerScene7(int id, FwGraphicsView* view);


protected:
    void keyPressEvent(FwKeyPressEvent *event);

private:
    FwPixmapBar* m_bar;
};

#endif // FIREWORKS_EXAMPLE_PLAYERSCENE7_H
