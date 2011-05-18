#ifndef PLAYERSCENE5_H
#define PLAYERSCENE5_H

#include "fwgui/fwscene.h"
//#include "fwgui/widgets/fwitemview.h"
#include "fwgui/widgets/fwcheckableitemview.h"


class PlayerScene5 : public FwScene
{
    Q_OBJECT
    typedef FwScene BaseClass;
public:
    PlayerScene5(int id, FwGraphicsView* view);

protected:
    void keyPressEvent(FwKeyPressEvent *event);
private:
       FwCheckableItemView* m_itemView;
};

#endif // PLAYERSCENE5_H
