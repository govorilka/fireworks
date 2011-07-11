#include "playerscene5.h"

#include "fwcore/fwml.h"

#include "fwgui/fwguievent.h"
#include "fwgui/fwgraphicsview.h"

#include "fwgui/primitives/fwrectprimitive.h"
#include "fwgui/primitives/fwstringprimitive.h"

#include "fwgui/widgets/fwcheckableitemview.h"
#include "fwgui/widgets/fwdigitinputwidget.h"

#include "fwutils/fwrequest.h"

PlayerScene5::PlayerScene5(FwGraphicsView* view) :
    BaseClass("playerScene5", view),
    m_itemView(new FwCheckableItemView("itemView", this)),
    m_digitInput(new FwDigitInputWidget("digitInput", this))
{
    m_itemView->show();

    connect(m_digitInput, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));
    m_digitInput->show();
}

void PlayerScene5::keyPressEvent(FwKeyPressEvent *event)
{
    int digit = event->digit();
    if(digit != -1)
    {
        QCoreApplication::sendEvent(m_digitInput, event);
        return;
    }

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
    case Qt::Key_PageUp:
    case Qt::Key_PageDown:
        QCoreApplication::sendEvent(m_itemView, event);
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

void PlayerScene5::valueChanged(int value)
{
    QList<FwPrimitive*> items = m_itemView->items();
    if(value >= 0 && value < items.count())
    {
        m_itemView->setCurrent(items.at(value));
    }
}
