#include "fwitemview.h"
#include "fwitemlayout.h"

#include "fwcore/fwml.h"

#include "fwgui/fwguievent.h"

#include "fwgui/primitives/fwprimitive.h"
#include "fwgui/primitives/fwstringprimitive.h"
#include "fwgui/primitives/fwtextprimitive.h"

FwItemView::FwItemView(const QByteArray& name, FwPrimitiveGroup* parent) :
    BaseClass(name, parent),
    m_layout(new FwLoopHSliderLayout(this)),
    m_current(0),
    m_currentDirty(false),
    m_previous(0),
    m_itemTemplate(0),
    m_highlight(0),
    needInitLayout(true),
    m_startItemsChanged(0),
    m_itemWidthDock(false),
    m_itemHeightDock(false),
    m_itemColor(0xFFFFFFFF),
    m_currentItemColor(0xFF000000)
{
    addLayoutClass(FwHSliderLayout::staticClassName, &FwHSliderLayout::constructor);
    addLayoutClass(FwVSliderLayout::staticClassName, &FwVSliderLayout::constructor);
    addLayoutClass(FwLoopHSliderLayout::staticClassName, &FwLoopHSliderLayout::constructor);
    addLayoutClass(FwLoopVSliderLayout::staticClassName, &FwLoopVSliderLayout::constructor);
    addLayoutClass(FwPagesLayout::staticClassName, &FwPagesLayout::constructor);
}

FwItemView::~FwItemView()
{
    m_items.clear();
    delete m_itemTemplate;
    delete m_layout;
}

void FwItemView::setItems(const QList<FwPrimitive*> items)
{
    prepareItemsChanged();

    m_previous = 0;
    m_current = 0;

    m_layout->resetAnimation();

    if(!m_items.isEmpty())
    {
        foreach(FwPrimitive* item, m_items)
        {
            if(!items.contains(item))
            {
                delete item;
            }
        }
        m_items.clear();
    }

    if(items.isEmpty())
    {
        if(m_highlight)
        {
            m_highlight->setVisible(false);
        }
    }
    else
    {
        foreach(FwPrimitive* item, items)
        {
            addItem(item);
        }
    }

    m_currentDirty = true;
    updateItems();
}

void FwItemView::setLayout(FwItemLayout* layout)
{
    if(layout && m_layout != layout)
    {
        prepareItemsChanged();
        delete m_layout;
        m_layout = layout;
        updateItems();
    }
}

bool FwItemView::addItem(FwPrimitive* item)
{
    if(item)
    {
        prepareItemsChanged();

        item->prepareGeometryChanged();

        item->setPosition(m_itemWidthDock ? Fw::HP_CenterDock : Fw::HP_Left,
                          m_itemHeightDock ? Fw::VP_MiddleDock : Fw::VP_Top);
        item->link(geometry());

        if(m_itemTemplate)
        {
            item->apply(m_itemTemplate);
        }
        item->currentChangedEvent(this, false);

        m_items.append(item);
        itemAddedEvent(item);

        if(!m_current)
        {
            setCurrent(item);
        }

        item->update();

        updateItems();

        return true;
    }
    return false;

}

bool FwItemView::addItem(FwPrimitive* item, const QVariant& data)
{
    if(item)
    {
        if(data.isValid())
        {
            item->setData(data);
        }
        addItem(item);
        return true;
    }
    return false;
}

FwTextPrimitive* FwItemView::addText(const QString& text, const QVariant& data)
{
    FwTextPrimitive* item = new FwTextPrimitive("", this);
    item->setText(text);
    addItem(item, data);
    return item;
}

FwStringPrimitive* FwItemView::addString(const QString& string, const QVariant& data)
{
    FwStringPrimitive* item = new FwStringPrimitive("", this);
    item->setString(string);
    addItem(item, data);
    return item;
}

void FwItemView::setCurrent(FwPrimitive* primitive)
{
    if(m_current != primitive)
    {
        prepareItemsChanged();

        m_previous = m_current;
        if(m_previous)
        {
            m_previous->currentChangedEvent(this, false);
        }

        m_current = primitive;
        m_currentDirty = true;

        updateItems(false);
    }
}

void FwItemView::setItemTemplate(FwMLObject* itemTemplate)
{
    if(m_itemTemplate != itemTemplate)
    {
        prepareItemsChanged();

        delete m_itemTemplate;
        m_itemTemplate = itemTemplate;
        if(itemTemplate)
        {
            foreach(FwPrimitive* item, m_items)
            {
                item->apply(m_itemTemplate);
                item->currentChangedEvent(this, false);
            }
            if(m_current)
            {
                m_current->currentChangedEvent(this, true);
            }
        }

        updateItems();
    }
}

void FwItemView::apply(FwMLObject *object)
{
    prepareItemsChanged();
    prepareGeometryChanged();

    bool bOk = false;

    FwMLObject* layoutNode = object->attribute("layout")->cast<FwMLObject>();
    if(layoutNode)
    {
        QByteArray layoutClass = layoutNode->className();
        if(!layoutClass.isEmpty() && m_layout->className() != layoutClass)
        {
            setLayout(createLayout(layoutClass));
        }
        m_layout->apply(layoutNode);
    }

    FwMLObject* itemNode = object->attribute("item")->cast<FwMLObject>();
    if(itemNode)
    {
        setItemTemplate(itemNode->clone()->cast<FwMLObject>());
    }

    FwMLNode* itemColorNode = object->attribute("itemColor");
    if(itemColorNode)
    {
        FwColor color = itemColorNode->toColor(&bOk);
        if(bOk)
        {
            setItemColor(color);
        }
    }

    FwMLNode* currentItemColorNode = object->attribute("currentItemColor");
    if(currentItemColorNode)
    {
        FwColor color = currentItemColorNode->toColor(&bOk);
        if(bOk)
        {
            setCurrentItemColor(color);
        }
    }

    FwMLObject* highlightNode = object->attribute("highlight")->cast<FwMLObject>();
    if(highlightNode && !m_highlight)
    {
        FwRectPrimitive* hightlight = new FwRectPrimitive("highlight", this);
        hightlight->setPosition(Fw::HP_CenterDock, Fw::VP_Top);
        setHighlight(hightlight);
    }

    FwMLNode* itemWidthDock = object->attribute("itemwidthdock");
    if(itemWidthDock)
    {
        bool dock = itemWidthDock->toBool(&bOk);
        if(bOk)
        {
            setItemWidthDock(dock);
        }
    }

    FwMLNode* itemHeightDock = object->attribute("itemheighthdock");
    if(itemHeightDock)
    {
        bool dock = itemHeightDock->toBool(&bOk);
        if(bOk)
        {
            setItemHeightDock(dock);
        }
    }

    BaseClass::apply(object);

    update();
    updateItems();
}

void FwItemView::geometryChangedEvent(const QRect &oldRect, QRect &rect)
{
    BaseClass::geometryChangedEvent(oldRect, rect);
    if(oldRect.size() != rect.size())
    {
        needInitLayout = true;
        updateChildrenRect(false, true);
    }
}

void FwItemView::childrenRectChangedEvent(bool posChanged, bool sizeChanged)
{
    if(m_current && (sizeChanged || needInitLayout))
    {
        m_layout->initItemsPos(m_items, m_current);
        needInitLayout = false;
    }
    BaseClass::childrenRectChangedEvent(posChanged, sizeChanged);
}

static QHash<QByteArray, FwLayoutConstructor*> layoutsFactory;

bool FwItemView::addLayoutClass(const QByteArray& className, FwLayoutConstructor* constructor)
{
    if(!className.isEmpty())
    {
        QByteArray cName = className.toLower();
        if(!layoutsFactory.contains(cName))
        {
            layoutsFactory.insert(cName, constructor);
            return true;
        }
    }
    return false;
}

FwItemLayout* FwItemView::createLayout(const QByteArray& className)
{
    if(!className.isEmpty())
    {
        FwLayoutConstructor* constructor = layoutsFactory.value(className.toLower(), 0);
        return constructor ? constructor(this) : 0;
    }
    return 0;
}

void FwItemView::keyPressEvent(FwKeyPressEvent* keyEvent)
{
    if(m_current)
    {
        switch(keyEvent->key())
        {
        case Qt::Key_Space:
        case Qt::Key_Return:
        case Qt::Key_Enter:
            m_current->trigger();
            itemTriggered(m_current);
            keyEvent->accept();
            break;

        default:
            if(m_layout->canNext())
            {
                FwPrimitive* candidatPrimitive = m_layout->nextItem(m_items, m_current, keyEvent);
                if(candidatPrimitive)
                {
                    setCurrent(candidatPrimitive);
                    keyEvent->accept();
                }
            }
        }
    }

    BaseClass::keyPressEvent(keyEvent);
}

void FwItemView::updateCurrent()
{
    if(m_current)
    {
        m_current->currentChangedEvent(this, true);
    }
    emit currentChanged(m_previous, m_current);
}

void FwItemView::setItemColor(const FwColor& color)
{
    if(m_itemColor != color)
    {
        prepareGeometryChanged();

        m_itemColor = color;
        foreach(FwPrimitive* primitive, m_items)
        {
            primitive->currentChangedEvent(this, false);
        }
        if(m_current)
        {
            m_current->currentChangedEvent(this, true);
        }
        update();
    }
}

void FwItemView::setCurrentItemColor(const FwColor& color)
{
    if(m_currentItemColor != color)
    {
        prepareGeometryChanged();

        m_currentItemColor = color;
        if(m_current)
        {
            m_current->currentChangedEvent(this, true);
        }

        update();
    }
}

void FwItemView::setHighlight(FwRectPrimitive* primitive)
{
    if(m_highlight != primitive)
    {
        prepareItemsChanged();
        delete m_highlight;
        m_highlight = primitive;
        updateItems();
    }
}

void FwItemView::updateItems(bool init)
{
    needInitLayout = needInitLayout || init;
    if((--m_startItemsChanged) == 0)
    {
        if(needInitLayout)
        {
            updateChildrenRect(false, true);
        }

        if(m_currentDirty)
        {
            if(m_highlight)
            {
                m_highlight->setVisible(m_current != 0);
            }
            if(!m_current || !m_previous || !m_layout->startAnimation(m_previous, m_current))
            {
                updateCurrent();
            }
            m_currentDirty = false;
        }
    }
}

void FwItemView::setItemWidthDock(bool enable)
{
    if(m_itemWidthDock != enable)
    {
        prepareItemsChanged();
        m_itemWidthDock = enable;
        updateItems(true);
    }
}

void FwItemView::setItemHeightDock(bool enable)
{
    if(m_itemHeightDock != enable)
    {
        prepareItemsChanged();
        m_itemHeightDock = enable;
        updateItems(true);
    }
}

void FwItemView::itemAddedEvent(FwPrimitive* item)
{
    Q_UNUSED(item);
}

void FwItemView::itemTriggered(FwPrimitive* item)
{
    Q_UNUSED(item);
}
