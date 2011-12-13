#include "fwcheckableitemview.h"

#include "fwcore/fwml.h"

#include "fwgui/fwscene.h"
#include "fwgui/fwgraphicsview.h"
#include "fwgui/fwimagelibrary.h"

#include "fwgui/primitives/fwpixmapprimitive.h"
#include "fwgui/primitives/fwstringprimitive.h"

FwCheckableItem::FwCheckableItem(FwCheckableItemView* parent) :
    BaseClass("", parent),
    m_parent(parent),
    m_caption(new FwStringPrimitive("caption", this)),
    m_checkBox(new FwPixmapPrimitive("checkbox", this)),
    m_check(false)
{
    updatePixmaps();
}

void FwCheckableItem::setCheck(bool value)
{
    if(m_check != (value += m_parent->isExclusive()))
    {
        m_check = value;
        if(m_parent->isExclusive())
        {
            foreach(FwCheckableItem* item, m_parent->checkableItems())
            {
                if(item != this && item->isChecked())
                {
                    item->m_check = false;
                    item->updatePixmaps();
                }
            }
        }
        updatePixmaps();
    }
}

void FwCheckableItem::updatePixmaps()
{
    m_checkBox->setPixmap(m_check ? m_parent->m_pixmapCheckOn : m_parent->m_pixmapCheckOff);
}

void FwCheckableItem::currentChangedEvent(FwItemView *view, bool current)
{
    m_caption->currentChangedEvent(view, current);
}

bool FwCheckableItem::trigger()
{
    bool check = m_check;
    setCheck(!isChecked());
    return check != m_check;
}

////////////////////////////////////////////////////////////////////////////////////////////

FwCheckableItemView::FwCheckableItemView(const QByteArray& name, FwPrimitiveGroup* parent) :
    BaseClass(name, parent),
    m_checkboxTemplate(0),
    m_exclusive(false)
{
}

FwCheckableItem* FwCheckableItemView::addItem(const QString& caption, bool check, const QVariant& data)
{
    FwCheckableItem* item = new FwCheckableItem(this);
    item->caption()->setString(caption);
    item->setCheck(check);
    item->setData(data);
    BaseClass::addItem(item);
    m_checkableItems.append(item);
    return item;
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
    FwPixmap pixmapCheckOn = scene()->view()->imageLibrary()->image(object, "pixmapCheckOn");
    if(!pixmapCheckOn.isNull())
    {
        setPixmapCheckOn(pixmapCheckOn);
    }

    FwPixmap pixmapCheckOff = scene()->view()->imageLibrary()->image(object, "pixmapCheckOff");
    if(!pixmapCheckOff.isNull())
    {
        setPixmapCheckOff(pixmapCheckOff);
    }

    FwMLBool* exclusiveNode = object->attribute("exclusive")->cast<FwMLBool>();
    if(exclusiveNode)
    {
        setExclusive(exclusiveNode->value());
    }

    BaseClass::apply(object);
}

void FwCheckableItemView::setExclusive(bool enable)
{
    if(m_exclusive != enable)
    {
        m_exclusive = enable;
        if(m_exclusive && !m_checkableItems.isEmpty())
        {
            FwCheckableItem* currentCheck = 0;
            foreach(FwCheckableItem* item, m_checkableItems)
            {
                if(item->isChecked())
                {
                    if(!currentCheck)
                    {
                        currentCheck = item;
                    }
                    else
                    {
                        item->setCheck(false);
                    }
                }
            }
            if(!currentCheck)
            {
                m_checkableItems.first()->setCheck(true);
            }
        }
    }
}
