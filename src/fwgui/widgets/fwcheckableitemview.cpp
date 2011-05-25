#include "fwcheckableitemview.h"

#include "fwcore/fwml.h"

#include "fwgui/primitives/fwpixmapprimitive.h"


FwCheckableItem::FwCheckableItem(FwPrimitive* item, FwCheckableItemView* parent) :
    m_check(false),
    m_item(item),
    m_parent(parent),
    m_checkBox(new FwPixmapPrimitive("", parent))
{
    m_checkBox->link(m_item->geometry());
}

////////////////////////////////////////////////////////////////////////////////////////////


FwCheckableItemView::FwCheckableItemView(const QByteArray& name, FwPrimitiveGroup* parent) :
    BaseClass(name, parent),
    m_checkboxTemplate(0)
{
}

void FwCheckableItemView::itemAddedEvent(FwPrimitive* item)
{
    FwCheckableItem* check = new FwCheckableItem(item, this);
    if(m_checkboxTemplate)
    {
        check->m_checkBox->apply(m_checkboxTemplate);
    }
    m_checkableItems.insert(item, check);
}

void FwCheckableItemView::itemTriggered(FwPrimitive* item)
{
    setCheck(item, !check(item));
}

bool FwCheckableItemView::check(FwPrimitive* item) const
{
    FwCheckableItem* check = m_checkableItems.value(item, 0);
    Q_ASSERT(check);
    return check->m_check;
}

void FwCheckableItemView::setCheck(FwPrimitive* item, bool value)
{
    FwCheckableItem* check = m_checkableItems.value(item, 0);
    Q_ASSERT(check);

    if(check && check->m_check != value)
    {
        check->m_check = value;
        check->m_checkBox->setPixmap(value ? m_pixmapCheckOn : m_pixmapCheckOff);
    }
}

void FwCheckableItemView::setPixmapCheckOn(const FwPixmap& pixmap)
{
    if(m_pixmapCheckOn != pixmap)
    {
        m_pixmapCheckOn = pixmap;
        foreach(FwCheckableItem* item, m_checkableItems)
        {
            if(item->m_check)
            {
                item->m_checkBox->setPixmap(m_pixmapCheckOn);
            }
        }
    }
}

void FwCheckableItemView::setPixmapCheckOff(const FwPixmap& pixmap)
{
    if(m_pixmapCheckOff != pixmap)
    {
        m_pixmapCheckOff = pixmap;
        foreach(FwCheckableItem* item, m_checkableItems)
        {
            if(!item->m_check)
            {
                item->m_checkBox->setPixmap(m_pixmapCheckOff);
            }
        }
    }
}

void FwCheckableItemView::apply(FwMLObject *object)
{
    FwMLObject* checkboxObject = object->attribute("checkbox")->cast<FwMLObject>();
    if(checkboxObject)
    {
        m_checkboxTemplate = checkboxObject->clone()->cast<FwMLObject>();
        foreach(FwCheckableItem* item, m_checkableItems)
        {
            item->m_checkBox->apply(m_checkboxTemplate);
        }
    }

    FwMLNode* pixmapCheckOnNode = object->attribute("pixmapCheckOn");
    if(pixmapCheckOnNode)
    {
        FwPixmap pixmapCheckOn = createPixmap(pixmapCheckOnNode);
        if(!pixmapCheckOn.isNull())
        {
            setPixmapCheckOn(pixmapCheckOn);
        }
    }

    FwMLNode* pixmapCheckOffNode = object->attribute("pixmapCheckOff");
    if(pixmapCheckOffNode)
    {
        FwPixmap pixmapCheckOff = createPixmap(pixmapCheckOffNode);
        if(!pixmapCheckOff.isNull())
        {
            setPixmapCheckOff(pixmapCheckOff);
        }
    }

    BaseClass::apply(object);
}
