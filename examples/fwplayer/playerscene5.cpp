#include "playerscene5.h"

#include "fwcore/fwml.h"

#include "fwgui/fwguievent.h"

#include "fwgui/fwgraphicsview.h"

#include "fwgui/primitives/fwrectprimitive.h"

PlayerScene5::PlayerScene5(int id, FwGraphicsView* view) :
    BaseClass(id, view),
    m_itemView(new FwCheckableItemView("itemView", this))
{
    m_itemView->prepareGeometryChanged();
    m_itemView->show();

    m_itemView->prepareItemsChanged();
    for(int i = 0; i < 100; i++)
    {
        m_itemView->addString("item" + QString::number(i));
    }
    m_itemView->updateItems();

    m_itemView->update();
}

void PlayerScene5::keyPressEvent(FwKeyPressEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Enter:
    case Qt::Key_Return:
        view()->setActiveScene(1);
        event->accept();
        return;
    case Qt::Key_Right:
    case Qt::Key_Left:
    case Qt::Key_Up:
    case Qt::Key_Down:
    case Qt::Key_Home:
    case Qt::Key_End:
        QCoreApplication::sendEvent(m_itemView, event);
        break;
    }
}
