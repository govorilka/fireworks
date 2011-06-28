#ifndef PLAYERSCENE4_H
#define PLAYERSCENE4_H

#include "fwgui/fwscene.h"
#include "fwgui/widgets/fwitemview.h"


class PlayerScene4 : public FwScene
{
    Q_OBJECT
    typedef FwScene BaseClass;
public:
    PlayerScene4(int id, FwGraphicsView* view);

protected:
    void keyPressEvent(FwKeyPressEvent *event);
private:
        FwItemView* m_itemView;
};

#endif // PLAYERSCENE4_H
