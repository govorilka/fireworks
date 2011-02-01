#include "menuscene.h"

#include "fwprimitives/fwslidingframeprimitive.h"
#include "fwprimitives/fwstringprimitive.h"

MenuScene::MenuScene(int id, FwGraphicsView* view) :
    BaseClass(id, view),
    m_slidingFrame(new FwSlidingFramePrimitive("slidingFrame", this)),
    m_item1(new FwStringPrimitive("item1", m_slidingFrame)),
    m_item2(new FwStringPrimitive("item2", m_slidingFrame)),
    m_item3(new FwStringPrimitive("item3", m_slidingFrame)),
    m_item4(new FwStringPrimitive("item4", m_slidingFrame)),
    m_item5(new FwStringPrimitive("item5", m_slidingFrame))
{
    m_slidingFrame->prepareGeometryChanged();
    m_slidingFrame->setPosition(Fw::HP_CenterDock, Fw::VP_Middle);
    m_slidingFrame->addItem(m_item1);
    m_slidingFrame->addItem(m_item2);
    m_slidingFrame->addItem(m_item3);
    m_slidingFrame->addItem(m_item4);
    m_slidingFrame->addItem(m_item5);
    m_item5->setString("12312312");
    m_item5->setString("12312312");
    m_item5->setString("123112");
    m_item5->setString("1212");
    m_item5->setString("12");
    m_slidingFrame->update();

    m_slidingFrame->setCurrent(m_item3);
}
