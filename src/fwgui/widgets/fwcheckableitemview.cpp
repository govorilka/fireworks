#include "fwcheckableitemview.h"

#include "fwcore/fwml.h"

#include "fwgui/primitives/fwpixmapprimitive.h"
#include "fwgui/primitives/fwstringprimitive.h"

FwCheckableItem::FwCheckableItem(FwCheckableItemView* parent) :
    BaseClass("", parent),
    m_check(false),
    m_parent(parent),
    m_caption(new FwStringPrimitive("caption", this)),
    m_checkBox(new FwPixmapPrimitive("checkbox", this))
{
}

void FwCheckableItem::setCheck(bool value)
{
    if(m_check != value)
    {
        m_check = value;
        updatePixmaps();
    }
}

void FwCheckableItem::updatePixmaps()
{
    m_checkBox->setPixmap(m_check ? m_parent->m_pixmapCheckOn : m_parent->m_pixmapCheckOff);
}

////////////////////////////////////////////////////////////////////////////////////////////

FwCheckableItemView::FwCheckableItemView(const QByteArray& name, FwPrimitiveGroup* parent) :
    BaseClass(name, parent),
    m_checkboxTemplate(0)
{
}

void FwCheckableItemView::addItem(FwCheckableItem* item)
{
    m_checkableItems.append(item);
    if(m_checkboxTemplate)
    {
        item->apply(m_checkboxTemplate);
    }
    BaseClass::addItem(item);
}

FwCheckableItem* FwCheckableItemView::addItem(const QString& caption, bool check, const QVariant& data)
{
    FwCheckableItem* item = new FwCheckableItem(this);
    item->caption()->setString(caption);
    item->setCheck(check);
    item->setData(data);
    addItem(item);
    return item;
}

void FwCheckableItemView::itemTriggered(FwPrimitive* item)
{
    static_cast<FwCheckableItem*>(item)->toggleCheck();
}

void FwCheckableItemView::setPixmapCheckOn(const FwPixmap& pixmap)
{
    if(m_pixmapCheckOn != pixmap)
    {
        m_pixmapCheckOn = pixmap;
        foreach(FwCheckableItem* item, m_checkableItems)
        {
            if(item->isChecked())
            {
                item->updatePixmaps();
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
            if(!item->isChecked())
            {
                item->updatePixmaps();
            }
        }
    }
}

void FwCheckableItemView::apply(FwMLObject *object)
{
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
