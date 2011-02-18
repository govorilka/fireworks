#include "fwitemview.h"
#include "fwitemlayout.h"

#include "fwcore/fwml.h"

#include "fwgui/primitives/fwprimitive.h"
#include "fwgui/primitives/fwstringprimitive.h"

FwItemView::FwItemView(const QByteArray& name, FwPrimitiveGroup* parent) :
    BaseClass(name, parent),
    m_layout(new FwSlidingFrameLayout(this)),
    m_current(0),
    m_itemTemplate(0)
{
    addLayoutClass(FwSlidingFrameLayout::staticClassName, &FwSlidingFrameLayout::constructor);
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
        updateChildrenRect();
        update();
    }
}

void FwItemView::addItem(FwPrimitive* primitive, const QVariant& data)
{
    Q_UNUSED(data);
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

        update();
    }
}

FwStringPrimitive* FwItemView::addItem(const QString& text, const QVariant& data)
{
    if(!text.isEmpty())
    {
        FwStringPrimitive* string = new FwStringPrimitive("", this);
        string->setString(text);
        addItem(string, data);
        return string;
    }
    return 0;
}

void FwItemView::setCurrent(FwPrimitive* primitive)
{
    if(m_current != primitive)
    {
        prepareGeometryChanged();
        m_current = primitive;
        updateChildrenRect();
        update();
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

        invalidateChildrenRect();
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

    BaseClass::apply(object);

    update();
}

void FwItemView::geometryChanged(const QRect &oldRect, QRect &rect)
{
    BaseClass::geometryChanged(oldRect, rect);
    if(m_current && m_layout)
    {
        m_layout->update(m_items, m_current, rect);
    }
}

void FwItemView::invalidateChildrenRect()
{
    if(m_current && m_layout)
    {
        m_layout->update(m_items, m_current, geometryRect());
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
