#include "playerscene2.h"

#include "fwcore/fwml.h"

#include "fwgui/fwguievent.h"
#include "fwgui/fwgraphicsview.h"

#include "fwgui/widgets/fwprogresswidget.h"

#include "fwgui/primitives/fwrectprimitive.h"
#include "fwgui/primitives/fwpixmapprimitive.h"

PlayerScene2::PlayerScene2(int id, FwGraphicsView* view) :
    BaseClass(id, view),
    m_rectPrimitive(new FwRectPrimitive("rectItem", this)),
    m_progressPrimitive(new FwProgressWidget("progressItem", this)),
    m_pixmapPrimitive(new FwPixmapPrimitive("pixmapItem", this))
{
    m_rectPrimitive->setPosition(Fw::HP_Center, Fw::VP_Middle);
    m_pixmapPrimitive->setPosition(Fw::HP_Center, Fw::VP_Middle);

    m_progressPrimitive->prepareGeometryChanged();
    m_progressPrimitive->setPosition(Fw::HP_Center, Fw::VP_Middle);
    m_progressPrimitive->setValue(0, 100, 50, false);
    m_progressPrimitive->show();
    m_progressPrimitive->update();
}

void PlayerScene2::keyPressEvent(FwKeyPressEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Enter:
    case Qt::Key_Return:
        view()->setActiveScene(3);
        event->accept();
        return;

    case Qt::Key_Left:
        m_progressPrimitive->setValue(m_progressPrimitive->value() - 10, isActive());
        event->accept();
        return;

    case Qt::Key_Right:
        m_progressPrimitive->setValue(m_progressPrimitive->value() + 10, isActive());
        event->accept();
        return;
    }
}
