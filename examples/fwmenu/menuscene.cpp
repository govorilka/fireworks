#include <QtCore/qdebug.h>

#include <QtTest/QtTest>

#include "menuscene.h"

#include "fwgui/fwguievent.h"
#include "fwgui/fwpainter.h"

#include "fwprimitives/fwslidingframeprimitive.h"
#include "fwprimitives/fwstringprimitive.h"

#include "qpgraphicsview.h"
#include "qprender.h"

MenuScene::MenuScene(int id, FwGraphicsView* view) :
    BaseClass(id, view),
    m_slidingFrame(new FwSlidingFramePrimitive("slidingFrame", this))
{
    m_slidingFrame->prepareGeometryChanged();
    m_slidingFrame->setPosition(Fw::HP_CenterDock, Fw::VP_Middle);

    for(int i = 0; i < 100; i++)
    {
        m_slidingFrame->addItem(QString("item") + QString::number(i));
    }

    m_slidingFrame->update();
}

void MenuScene::keyPressEvent(FwKeyPressEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Left:
        m_slidingFrame->setCurrent(Fw::goPtr(m_slidingFrame->items(), Fw::Go_Previous, m_slidingFrame->current()));
        event->accept();
        break;

    case Qt::Key_Right:
        m_slidingFrame->setCurrent(Fw::goPtr(m_slidingFrame->items(), Fw::Go_Next, m_slidingFrame->current()));
        event->accept();
        break;
     }
}

void MenuScene::testCase1()
{
    QBENCHMARK
    {
        //m_slidingFrame->updateLayout(m_slidingFrame->items(), m_slidingFrame->current());
        m_slidingFrame->setCurrent(Fw::goPtr(m_slidingFrame->items(), Fw::Go_Previous, m_slidingFrame->current()));
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
