#include "playerscene.h"

#include "fwcore/fwml.h"

#include "fwprimitives/fwrectprimitive.h"
#include "fwprimitives/fwstringprimitive.h"
#include "fwprimitives/fwpixmapprimitive.h"

PlayerScene::PlayerScene(int id, FwGraphicsView* view) :
    BaseClass(id, view),
    m_rectPrimitive(new FwRectPrimitive("rectItem", this)),
    m_stringPrimitive(new FwStringPrimitive("stringItem", this)),
    m_pixmapPrimitive(new FwPixmapPrimitive("pixmapItem", this))
{
    m_rectPrimitive->setPosition(Fw::HP_Center, Fw::VP_Middle);
    m_pixmapPrimitive->setPosition(Fw::HP_Center, Fw::VP_Middle);
    m_stringPrimitive->setPosition(Fw::HP_Center, Fw::VP_Middle);
}
