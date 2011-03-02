#include "fwitemview.h"
#include "fwitemlayout.h"

#include "fwcore/fwml.h"

#include "fwgui/fwguievent.h"

#include "fwgui/primitives/fwprimitive.h"
#include "fwgui/primitives/fwstringprimitive.h"

FwItemView::FwItemView(const QByteArray& name, FwPrimitiveGroup* parent) :
    BaseClass(name, parent),
    m_layout(new FwLoopHSliderLayout(this)),
    m_current(0),
    m_previous(0),
    m_itemTemplate(0),
    needInitLayout(true)
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

void FwItemView::setLayout(FwItemLayout* layout)
{
    if(m_layout != layout)
    {
        prepareGeometryChanged();
        delete m_layout;
        m_layout = layout;
        needInitLayout = true;
        updateChildrenRect();
        update();
    }
}

void FwItemView::addItem(FwPrimitive* primitive)
{
    if(!m_items.contains(primitive))
    {
        prepareGeometryChanged();

        primitive->link(geometry());
        if(m_itemTemplate)
        {
            primitive->apply(m_itemTemplate);
        }
        m_items.append(primitive);

        if(!m_current)
        {
            m_current = primitive;
        }

        needInitLayout = true;
        updateChildrenRect();

        update();
    }
}

FwStringPrimitive* FwItemView::addItem(const QString& text, const QVariant& data)
{
    if(!text.isEmpty())
    {
        FwStringPrimitive* string = new FwStringPrimitive("", this);
        string->setString(text);
        if(data.isValid())
        {
            string->setData(data);
        }
        addItem(string);
        return string;
    }
    return 0;
}

void FwItemView::setCurrent(FwPrimitive* primitive)
{
    if(m_current != primitive)
    {
        if(m_layout)
        {
            m_layout->setCurrent(m_previous, m_current, isVisibleOnScreen());
        }
        else
        {
            startChangedCurrent();
            applyCurrentItem(primitive);
        }
    }
}

void FwItemView::setItemTemplate(FwMLObject* itemTemplate)
{
    if(m_itemTemplate != itemTemplate)
    {
        prepareGeometryChanged();

        delete m_itemTemplate;
        m_itemTemplate = itemTemplate;
        foreach(FwPrimitive* item, m_items)
        {
            item->apply(m_itemTemplate);
        }

        needInitLayout = true;
        updateChildrenRect();

        update();
    }
}

void FwItemView::apply(FwMLObject *object)
{
    prepareGeometryChanged();

    FwMLObject* layoutNode = object->attribute("layout")->cast<FwMLObject>();
    if(layoutNode)
    {
        FwItemLayout* layout = this->layout();
        QByteArray layoutClass = layoutNode->className();
        if(!layoutClass.isEmpty() && (!layout || layout->className() != layoutClass))
        {
            layout = createLayout(layoutClass);
            setLayout(layout);
        }
        if(layout)
        {
            layout->apply(layoutNode);
        }
    }

    FwMLObject* itemNode = object->attribute("item")->cast<FwMLObject>();
    if(itemNode)
    {
        setItemTemplate(itemNode->clone()->cast<FwMLObject>());
    }

    FwMLNode* itemColorNode = object->attribute("itemColor");
    if(itemColorNode)
    {
        bool bOk = false;
        FwColor color = itemColorNode->toColor(&bOk);
        if(bOk)
        {
            setItemColor(color);
        }
    }

    FwMLNode* currentItemColorNode = object->attribute("currentItemColor");
    if(currentItemColorNode)
    {
        bool bOk = false;
        FwColor color = currentItemColorNode->toColor(&bOk);
        if(bOk)
        {
            setCurrentItemColor(color);
        }
    }

    BaseClass::apply(object);

    update();
}

void FwItemView::geometryChangedEvent(const QRect &oldRect, QRect &rect)
{
    BaseClass::geometryChangedEvent(oldRect, rect);
    needInitLayout = (oldRect.size() != rect.size());
    updateChildrenRect();
}

void FwItemView::invalidateChildrenRect()
{
    if(m_current && m_layout)
    {
        if(childSizeChanged || needInitLayout)
        {
            m_layout->init(m_items, geometryRect());
            m_layout->update(m_items, m_current, geometryRect());
            needInitLayout = false;
        }
    }
    BaseClass::invalidateChildrenRect();
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
    if(m_current && m_layout)
    {
        m_layout->keyPressEvent(m_items, keyEvent);
    }
}

void FwItemView::startChangedCurrent()
{
    if(m_current)
    {
        m_current->setPenColor(m_itemColor);
    }
}

void FwItemView::applyCurrentItem(FwPrimitive* primitive)
{
    m_previous = m_current;
    m_current = primitive;
    m_current->setPenColor(m_currentItemColor);
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
            primitive->setPenColor(m_itemColor);
        }
        if(m_current)
        {
            m_current->setPenColor(m_currentItemColor);
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
            m_current->setPenColor(m_currentItemColor);
        }

        update();
    }
}
