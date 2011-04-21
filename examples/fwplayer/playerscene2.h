#ifndef PLAYERSCENE2_H
#define PLAYERSCENE2_H

#include "fwgui/fwscene.h"

class FwRectPrimitive;
class FwStringPrimitive;
class FwPixmapPrimitive;

class PlayerScene2 : public FwScene
{
    typedef FwScene BaseClass;

public:
    PlayerScene2(int id, FwGraphicsView* view);

protected:
    void keyPressEvent(FwKeyPressEvent *event);

private:
    FwRectPrimitive* m_rectPrimitive;
    FwStringPrimitive* m_stringPrimitive;
    FwPixmapPrimitive* m_pixmapPrimitive;
};

#endif // PLAYERSCENE2_H
