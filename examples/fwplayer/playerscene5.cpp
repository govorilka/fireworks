#include "playerscene5.h"

#include "fwcore/fwml.h"

#include "fwgui/fwguievent.h"
#include "fwgui/fwgraphicsview.h"

#include "fwgui/primitives/fwrectprimitive.h"
#include "fwgui/primitives/fwstringprimitive.h"

#include "fwgui/widgets/fwcheckableitemview.h"

#include "fwutils/fwrequest.h"

PlayerScene5::PlayerScene5(FwGraphicsView* view) :
    BaseClass("playerScene5", view),
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
        {
            FwRequest request(this, tr("Are you sure?"));

            FwRequestAnswer& answerOk = request.addAnswer(FwRequestAnswer::SR_Ok,"ok");
            answerOk.setCaption("Ok");
            answerOk.setKey(Qt::Key_Return);

            FwRequestAnswer& answerCancel = request.addAnswer(FwRequestAnswer::SR_Ok,"cancel");
            answerCancel.setCaption("Cancel");
            answerCancel.setKey(Qt::Key_Escape);

            request.postRequest(view());
        }
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

    case Qt::Key_1:
        m_itemView->setVisible(!m_itemView->isVisible());
        event->accept();
        break;
    }
}

void PlayerScene5::apply(FwMLObject *object)
{
    BaseClass::apply(object);

    m_itemView->prepareItemsChanged();
    for(int i = 0; i < 100; i++)
    {
        m_itemView->addItem("item" + QString::number(i), false);
    }
    m_itemView->updateItems();
}

void PlayerScene5::showEvent(FwSceneShowEvent *event)
{
    BaseClass::showEvent(event);
}

void PlayerScene5::requestAcceptEvent(FwResult* result)
{
    if(result->result() == FwRequestAnswer::SR_Ok)
    {
        view()->setActiveScene("playerScene6");
    }
}
