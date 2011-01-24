#ifndef PLAYERSCENE_H
#define PLAYERSCENE_H

#include "fwgui/fwscene.h"

class FwRectPrimitive;
class FwStringPrimitive;

class PlayerScene : public FwScene
{
    typedef FwScene BaseClass;

public:
    PlayerScene(int id, FwGraphicsView* view);

    void apply(FwMLObject *object);

private:
    FwRectPrimitive* m_rectPrimitive;
    FwStringPrimitive* m_stringPrimitive;
};

#endif // PLAYERSCENE_H
