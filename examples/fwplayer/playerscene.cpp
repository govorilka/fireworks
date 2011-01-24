#include "playerscene.h"

#include "fwcore/fwml.h"

#include "fwprimitives/fwrectprimitive.h"
#include "fwprimitives/fwstringprimitive.h"

PlayerScene::PlayerScene(int id, FwGraphicsView* view) :
    BaseClass(id, view),
    m_rectPrimitive(0),
    m_stringPrimitive(0)
{
    m_rectPrimitive = new FwRectPrimitive(this);
    m_rectPrimitive->anchor()->setPolicy(FwAnchor::HP_Center, FwAnchor::VP_Middle);

    m_stringPrimitive = new FwStringPrimitive(this);
    m_stringPrimitive->anchor()->setPolicy(FwAnchor::HP_Center, FwAnchor::VP_Middle);
}

void PlayerScene::apply(FwMLObject *object)
{
    prepareGeometryChanged();

    FwMLObject* rectObject = object->attribute("rectItem")->cast<FwMLObject>();
    if(rectObject)
    {
        m_rectPrimitive->apply(rectObject);
    }

    FwMLObject* stringObject = object->attribute("stringItem")->cast<FwMLObject>();
    if(stringObject)
    {
        m_stringPrimitive->apply(stringObject);
    }


    BaseClass::apply(object);

    FwPrimitiveGroup::update();
}
