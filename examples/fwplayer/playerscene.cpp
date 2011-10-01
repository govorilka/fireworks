#include "playerscene.h"

#include "fwcore/fwml.h"

#include "fwgui/fwguievent.h"
#include "fwgui/fwgraphicsview.h"

#include "fwgui/primitives/fwrectprimitive.h"
#include "fwgui/primitives/fwstringprimitive.h"
#include "fwgui/primitives/fwpixmapprimitive.h"

PlayerScene::PlayerScene(FwGraphicsView* view) :
    BaseClass("playerScene1", view),
    m_rectPrimitive(new FwRectPrimitive("rectItem", this)),
    m_stringPrimitive(new FwStringPrimitive("stringItem", this)),
    m_pixmapPrimitive(new FwPixmapPrimitive("pixmapItem", this)),
    m_linePrimitive(new FwLinePrimitive("lineItem", this))
{
    m_rectPrimitive->setPosition(Fw::HP_Center, Fw::VP_Middle);
    m_pixmapPrimitive->setPosition(Fw::HP_Center, Fw::VP_Middle);
    m_stringPrimitive->link(m_rectPrimitive->geometry());

    m_linePrimitive->setLine(0, 0, 250, 0);
    m_linePrimitive->link(m_stringPrimitive->geometry());
}

void PlayerScene::keyPressEvent(FwKeyPressEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Enter:
    case Qt::Key_Return:
        view()->setActiveScene("playerScene2");
        event->accept();
        return;

    case Qt::Key_1:
        m_stringPrimitive->setVisible(!m_stringPrimitive->isVisible());
        event->accept();
        return;

    case Qt::Key_2:
        m_pixmapPrimitive->setVisible(!m_pixmapPrimitive->isVisible());
        event->accept();
        return;

    default:
        break;
    }
}
