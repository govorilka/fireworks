#include "playerscene2.h"

#include "fwcore/fwml.h"

#include "fwgui/fwguievent.h"
#include "fwgui/fwgraphicsview.h"

#include "fwgui/primitives/fwrectprimitive.h"
#include "fwgui/primitives/fwstringprimitive.h"
#include "fwgui/primitives/fwpixmapprimitive.h"

PlayerScene2::PlayerScene2(int id, FwGraphicsView* view) :
    BaseClass(id, view),
    m_rectPrimitive(new FwRectPrimitive("rectItem", this)),
    m_stringPrimitive(new FwStringPrimitive("stringItem", this)),
    m_pixmapPrimitive(new FwPixmapPrimitive("pixmapItem", this))
{
    m_rectPrimitive->setPosition(Fw::HP_Center, Fw::VP_Middle);
    m_pixmapPrimitive->setPosition(Fw::HP_Center, Fw::VP_Middle);
    m_stringPrimitive->setPosition(Fw::HP_Center, Fw::VP_Middle);
}

void PlayerScene2::keyPressEvent(FwKeyPressEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        view()->setActiveScene(1);
        event->accept();
    }
}
