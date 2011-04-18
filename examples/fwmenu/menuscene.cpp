#include <QtCore/qdebug.h>

#include <QtTest/QtTest>

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
    m_itemView->setPosition(Fw::HP_Center, Fw::VP_Middle);

    for(int i = 0; i < 100; i++)
    {
        m_itemView->addString(QString("item") + QString::number(i));
    }

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
    }
}

void MenuScene::testCase1()
{
    QBENCHMARK
    {
        m_itemView->setCurrent(Fw::goPtr(m_itemView->items(), Fw::Go_Previous, m_itemView->current()));
    }
}

void MenuScene::testCase2()
{
    QImage img(size(), QImage::Format_ARGB32);
    FwPainter painter(QRect(QPoint(0, 0), size()), rect(), new QPRender(&img));
    QBENCHMARK
    {
        view()->render(&painter, rect());
    }
}

void MenuScene::testCase3()
{
    FwItemLayout* layout = m_itemView->layout();
    if(!layout)
    {
        return;
    }

    QBENCHMARK
    {
        //layout->update(m_itemView->items(), m_itemView->current(), m_itemView->geometryRect());
    }
}
