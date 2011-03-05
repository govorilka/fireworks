#include <QtCore/qcoreapplication.h>
#include <QtCore/qdebug.h>
#include <QtCore/qdatetime.h>

#include "menuscene.h"

#include "fwgui/fwguievent.h"
#include "fwgui/fwpainter.h"

#include "fwgui/widgets/fwitemview.h"
#include "fwgui/widgets/fwitemlayout.h"

#include "fwgui/primitives/fwstringprimitive.h"

#include "qpgraphicsview.h"
#include "qprender.h"

MenuScene::MenuScene(int id, FwGraphicsView* view) :
    BaseClass(id, view),
    m_itemView(new FwItemView("itemView", this))
{
    m_itemView->prepareGeometryChanged();
    m_itemView->show();
    m_itemView->setPosition(Fw::HP_Center, Fw::VP_MiddleDock);
    m_itemView->update();
}

void MenuScene::keyPressEvent(FwKeyPressEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Right:
    case Qt::Key_Left:
    case Qt::Key_Up:
    case Qt::Key_Down:
    case Qt::Key_Home:
    case Qt::Key_End:
        QCoreApplication::sendEvent(m_itemView, event);
        break;

    case Qt::Key_1:
        {
            QTime currentTime = QTime::currentTime();
            m_itemView->prepareItemsChanged();
            qDebug() << "MenuScene::keyPressEvent: start";
            for(int i = 0; i < 10000; i++)
            {
                m_itemView->addItem(QString("item") + QString::number(i));
            }
            m_itemView->updateItems();
            qDebug() << "MenuScene::keyPressEvent" << currentTime.msecsTo(QTime::currentTime());
        }
        event->accept();
        break;

    case Qt::Key_2:
        m_itemView->clear();
        event->accept();
        break;

    }
}
