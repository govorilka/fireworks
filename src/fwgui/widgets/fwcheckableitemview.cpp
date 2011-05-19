#include "fwcheckableitemview.h"
#include "fwgui/primitives/fwpixmapprimitive.h"



FwCheckableItem::FwCheckableItem(FwPrimitive* item, FwCheckableItemView* parent) :
    m_check(false),
    m_item(item),
    m_parent(parent),
    m_checkBox(new FwPixmapPrimitive("", parent))
{
}

////////////////////////////////////////////////////////////////////////////////////////////


FwCheckableItemView::FwCheckableItemView(const QByteArray& name, FwPrimitiveGroup* parent) :
    BaseClass(name, parent)
{
}

void FwCheckableItemView::itemAddedEvent(FwPrimitive* item)
{
    m_checkableItems.insert(item, new FwCheckableItem(item, this));
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
