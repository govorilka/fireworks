#ifndef PLAYERSCENE3_H
#define PLAYERSCENE3_H

#include "fwgui/fwscene.h"

class PlayerScene3 : public FwScene
{
    Q_OBJECT
    typedef FwScene BaseClass;

public:
     PlayerScene3(int id, FwGraphicsView* view);

protected:
    void keyPressEvent(FwKeyPressEvent *event);

private:
    FwPrimitiveGroup* m_PrimitiveGroup;
    FwRectPrimitive* m_rectPrimitive1;
    FwRectPrimitive* m_rectPrimitive2;
    FwRectPrimitive* m_rectPrimitive3;
};

#endif // PLAYERSCENE3_H
