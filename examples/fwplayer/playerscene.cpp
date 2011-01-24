#include "playerscene.h"

#include "fwcore/fwml.h"

#include "fwprimitives/fwrectprimitive.h"

PlayerScene::PlayerScene(int id, FwGraphicsView* view) :
    BaseClass(id, view),
    m_rectPrimitive(0)
{
    m_rectPrimitive = new FwRectPrimitive(this);
}

void PlayerScene::apply(FwMLObject *object)
{
    prepareGeometryChanged();

    FwMLObject* rectObject = object->attribute("rectItem")->cast<FwMLObject>();
    if(rectObject)
    {
        m_rectPrimitive->apply(rectObject);
    }

    BaseClass::apply(object);

    FwPrimitiveGroup::update();
}
