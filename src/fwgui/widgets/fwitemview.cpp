#include "fwitemview.h"
#include "fwitemlayout.h"

#include "fwcore/fwml.h"

#include "fwgui/fwguievent.h"
#include "fwgui/fwguifactory.h"
#include "fwgui/fwscene.h"
#include "fwgui/fwgraphicsview.h"
#include "fwgui/fwimagelibrary.h"

#include "fwgui/primitives/fwprimitive.h"
#include "fwgui/primitives/fwstringprimitive.h"
#include "fwgui/primitives/fwtextprimitive.h"
#include "fwgui/primitives/fwpixmapprimitive.h"

#include "fwgui/widgets/fwitemview.h"

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
    m_currentItemColor(0xFF000000),
    m_leftArrow(0),
    m_rightArrow(0),
    m_centerPixmap(0)
{
    addLayoutClass(FwHSliderLayout::staticClassName, &FwHSliderLayout::constructor);
    addLayoutClass(FwVSliderLayout::staticClassName, &FwVSliderLayout::constructor);
    addLayoutClass(FwLoopHSliderLayout::staticClassName, &FwLoopHSliderLayout::constructor);
    addLayoutClass(FwLoopVSliderLayout::staticClassName, &FwLoopVSliderLayout::constructor);
    addLayoutClass(FwPagesLayout::staticClassName, &FwPagesLayout::constructor);
}

FwItemView::~FwItemView()
{
    clear();
    delete m_itemTemplate;
    delete m_layout;
}

void FwItemView::setItems(const QList<FwPrimitive*> items)
{
    prepareItemsChanged();

    m_previous = 0;
    m_current = 0;

    m_layout->resetAnimation();
    m_layout->cleanUp();

    if(!m_items.isEmpty())
    {
        foreach(FwPrimitive* item, m_items)
        {
            if(!items.contains(item))
            {
                item->m_itemView = 0;
                delete item;
            }
        }
        m_items.clear();
    }

    if(m_highlight)
    {
        m_highlight->setVisible(!items.isEmpty());
    }

    foreach(FwPrimitive* item, items)
    {
        addItem(item);
    }

    m_currentDirty = true;
    needInitLayout = true;

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

        item->m_itemView = this;

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
        QByteArray layoutBase = layoutNode->baseName();
        if(!layoutBase.isEmpty() && m_layout->className() != layoutBase)
        {
            setLayout(createLayout(layoutBase));
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
        FwRectPrimitive* hightlight = FwGuiFactory::createRectPrimitive(highlightNode->baseName(), "highlight", this);
        if(hightlight)
        {
            hightlight->setPosition(Fw::HP_CenterDock, Fw::VP_Top);
            setHighlight(hightlight);
        }
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

    FwMLObject* leftArrowNode = object->attribute("leftArrow")->cast<FwMLObject>();
    if(leftArrowNode)
    {
        m_leftArrow = new FwPixmapPrimitive("leftArrow", this);
    }

    FwMLObject* rightArrowNode = object->attribute("rightArrow")->cast<FwMLObject>();
    if(rightArrowNode)
    {
        m_rightArrow = new FwPixmapPrimitive("rightArrow", this);
    }

    FwPixmap centerPixmap = scene()->view()->imageLibrary()->image(object, "centerPixmap");
    if(!centerPixmap.isNull())
    {
        m_centerPixmap = new FwPixmapPrimitive("centerPixmap", this);
        m_centerPixmap->prepareGeometryChanged();
        m_centerPixmap->setPosition(Fw::HP_Center, Fw::VP_Middle);
        m_centerPixmap->setPixmap(centerPixmap);
        m_centerPixmap->update();
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
        updateChildren();
    }
}

void FwItemView::childrenRectChangedEvent(bool posChanged, bool sizeChanged)
{
    if(m_current && needInitLayout)
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
        switch(keyEvent->key().qtKey())
        {
        case Qt::Key_Space:
        case Qt::Key_Return:
        case Qt::Key_Enter:
            if(m_current->trigger())
            {
                emit itemTriggered(m_current);
                keyEvent->accept();
            }
            return;

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

void FwItemView::updateCurrent(bool updateLayout)
{
    if(m_current)
    {
        m_current->currentChangedEvent(this, true);
        if(updateLayout)
        {
            m_layout->update(m_items, m_current, m_geometry->rect());
        }
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
            if(m_current)
            {
                m_layout->initItemsPos(m_items, m_current);
            }
            updateChildren();
        }

        if(m_currentDirty)
        {
            if(m_highlight)
            {
                m_highlight->setVisible(m_current != 0);
            }

            if(needInitLayout ||
               !visibleOnScreen ||
               !m_current ||
               !m_previous ||
               !m_layout->startAnimation(m_previous, m_current))
            {
                updateCurrent(true);
            }

            m_currentDirty = false;
        }

        needInitLayout = false;
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
