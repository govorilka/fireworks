#include "playerscene3.h"

#include "fwcore/fwml.h"

#include "fwgui/fwguievent.h"

#include "fwgui/fwgraphicsview.h"

#include "fwgui/primitives/fwrectprimitive.h"



PlayerScene3::PlayerScene3(int id, FwGraphicsView* view) :
    BaseClass(id, view),
    m_PrimitiveGroup(new FwPrimitiveGroup("rectItemGroup",this)),
    m_rectPrimitive1(new FwRectPrimitive("rectItem1", m_PrimitiveGroup)),
    m_rectPrimitive2(new FwRectPrimitive("rectItem2", m_PrimitiveGroup)),
    m_rectPrimitive3(new FwRectPrimitive("rectItem3", m_PrimitiveGroup))
{
}

void PlayerScene3::keyPressEvent(FwKeyPressEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Enter:
    case Qt::Key_Return:
        view()->setActiveScene(4);
        event->accept();
        return;
    }
}
