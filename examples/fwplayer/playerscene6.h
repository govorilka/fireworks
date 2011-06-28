#ifndef FIREWORKS_EXAMPLE_PLAYERSCENE6_H
#define FIREWORKS_EXAMPLE_PLAYERSCENE6_H

#include "fwgui/fwscene.h"
#include "fwgui/widgets/fwitemview.h"

class PlayerScene6 : public FwScene
{
    Q_OBJECT
    typedef FwScene BaseClass;

public:
    PlayerScene6(int id, FwGraphicsView* view);

protected:
    void keyPressEvent(FwKeyPressEvent *event);

private:
    FwItemView* m_itemView;
};

#endif // FIREWORKS_EXAMPLE_PLAYERSCENE6_H
