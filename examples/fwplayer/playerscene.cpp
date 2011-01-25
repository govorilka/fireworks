#include "playerscene.h"

#include "fwcore/fwml.h"

#include "fwprimitives/fwrectprimitive.h"
#include "fwprimitives/fwstringprimitive.h"
#include "fwprimitives/fwpixmapprimitive.h"

PlayerScene::PlayerScene(int id, FwGraphicsView* view) :
    BaseClass(id, view),
    m_rectPrimitive(0),
    m_stringPrimitive(0),
    m_pixmapPrimitive(0)
{
    m_rectPrimitive = new FwRectPrimitive(this);
    m_rectPrimitive->anchor()->setPolicy(FwAnchor::HP_Center, FwAnchor::VP_Middle);

    m_pixmapPrimitive = new FwPixmapPrimitive(this);
    m_pixmapPrimitive->anchor()->setPolicy(FwAnchor::HP_Center, FwAnchor::VP_Middle);

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

    FwMLObject* pixmapObject = object->attribute("pixmapItem")->cast<FwMLObject>();
    if(pixmapObject)
    {
        m_pixmapPrimitive->apply(pixmapObject);
    }

    FwMLObject* stringObject = object->attribute("stringItem")->cast<FwMLObject>();
    if(stringObject)
    {
        m_stringPrimitive->apply(stringObject);
    }


    BaseClass::apply(object);

    FwPrimitiveGroup::update();
}
