#ifndef PLAYERSCENE_H
#define PLAYERSCENE_H

#include "fwgui/fwscene.h"

class FwRectPrimitive;
class FwStringPrimitive;
class FwPixmapPrimitive;
class FwLinePrimitive;

class PlayerScene : public FwScene
{
    Q_OBJECT
    typedef FwScene BaseClass;

public:
    PlayerScene(FwGraphicsView* view);

protected:
    void keyPressEvent(FwKeyPressEvent *event);

private:
    FwRectPrimitive* m_rectPrimitive;
    FwStringPrimitive* m_stringPrimitive;
    FwPixmapPrimitive* m_pixmapPrimitive;
    FwLinePrimitive* m_linePrimitive;
};

#endif // PLAYERSCENE_H
