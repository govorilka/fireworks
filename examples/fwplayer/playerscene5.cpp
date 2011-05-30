#include "playerscene5.h"

#include "fwcore/fwml.h"

#include "fwgui/fwguievent.h"
#include "fwgui/fwgraphicsview.h"

#include "fwgui/primitives/fwrectprimitive.h"
#include "fwgui/primitives/fwstringprimitive.h"

#include "fwgui/widgets/fwcheckableitemview.h"

PlayerScene5::PlayerScene5(int id, FwGraphicsView* view) :
    BaseClass(id, view),
    m_itemView(new FwCheckableItemView("itemView", this))
{
    m_itemView->prepareGeometryChanged();
    m_itemView->show();
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
    case Qt::Key_Space:
        QCoreApplication::sendEvent(m_itemView, event);
        break;
    }
}

void PlayerScene5::apply(FwMLObject *object)
{
    BaseClass::apply(object);

    m_itemView->prepareItemsChanged();
    for(int i = 0; i < 5; i++)
    {
        m_itemView->addItem("item" + QString::number(i), false);
    }
    m_itemView->updateItems();
}

void PlayerScene5::showEvent(FwSceneShowEvent *event)
{
    BaseClass::showEvent(event);
}

void PlayerScene5::invalidateChildrenRect()
{
    BaseClass::invalidateChildrenRect();

    qDebug() << "=================================================: ";
    qDebug() << "Scene geometry rect: " << geometry()->rect();

    foreach(FwCheckableItem* item, m_itemView->checkableItems())
    {
        qDebug() << "PlayerScene5::apply" << item->caption()->string() << item->pos() << item->geometry()->rect();
        foreach(FwPrimitive* child, item->primitives())
        {
            qDebug() << "child" << child->name() << ": " << child->geometry()->rect();
        }
    }
}
