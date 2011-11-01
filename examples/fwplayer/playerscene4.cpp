#include "playerscene4.h"

#include "fwcore/fwml.h"

#include "fwgui/fwguievent.h"

#include "fwgui/fwgraphicsview.h"

#include "fwgui/primitives/fwrectprimitive.h"


PlayerScene4::PlayerScene4(FwGraphicsView* view) :
    BaseClass("playerScene4", view),
    m_itemView(new FwItemView("itemView", this))
{
    m_itemView->prepareGeometryChanged();
    m_itemView->show();

    char c = 'A';
    QString caption = QChar(c);

    for(int i = 0; i < 100; i++)
    {
        m_itemView->addText(caption, false);
        if(c == 'Z')
        {
            c = 'A';
            caption = QChar(c);
        }
        else
        {
            caption += ' ';
            caption += (++c);
        }
    }

    m_itemView->update();
}

void PlayerScene4::keyPressEvent(FwKeyPressEvent *event)
{
    switch(event->key().qtKey())
    {
    case Qt::Key_Enter:
    case Qt::Key_Return:
        view()->setActiveScene("playerScene5");
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
