#ifndef FIREWORKS_ITEMVIEW_H
#define FIREWORKS_ITEMVIEW_H

#include <QtCore/qlist.h>
#include <QtCore/qhash.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>

#include "fwgui/widgets/fwwidget.h"

class FwItemView;
class FwItemLayout;
class FwPrimitive;
class FwStringPrimitive;
class FwTextPrimitive;

typedef FwItemLayout*(FwLayoutConstructor)(FwItemView* view);

class FIREWORKSSHARED_EXPORT FwItemView : public FwWidget
{
    Q_OBJECT
    typedef FwWidget BaseClass;

public:
    friend class FwPrimitive;
    friend class FwItemLayout;

    FwItemView(const QByteArray& name, FwPrimitiveGroup* parent);
    ~FwItemView();

    inline QList<FwPrimitive*> items() const;
    void setItems(const QList<FwPrimitive*> items);
    inline void clear();

    virtual bool addItem(FwPrimitive* item);
    bool addItem(FwPrimitive* item, const QVariant& data);
    FwStringPrimitive* addString(const QString& string, const QVariant& data = QVariant());
    FwTextPrimitive* addText(const QString& text, const QVariant& data = QVariant());

    inline void prepareItemsChanged();
    void updateItems(bool init = true);

    inline FwPrimitive* current() const;
    inline FwPrimitive* previous() const;
    void setCurrent(FwPrimitive* primitive);

    inline FwItemLayout* layout() const;
    void setLayout(FwItemLayout* layout);

    inline FwMLObject* itemTemplate() const;
    void setItemTemplate(FwMLObject* itemTemplate);

    inline FwColor itemColor() const;
    void setItemColor(const FwColor& color);

    inline FwColor currentItemColor() const;
    void setCurrentItemColor(const FwColor& color);

    inline FwRectPrimitive* highlight() const;
    void setHighlight(FwRectPrimitive* primitive);

    void apply(FwMLObject *object);

    static bool addLayoutClass(const QByteArray& className, FwLayoutConstructor* constructor);
    FwItemLayout* createLayout(const QByteArray& className);

    inline bool isItemWidthDockEnable() const;
    void setItemWidthDock(bool enable);

    inline bool isItemHeightDockEnable() const;
    void setItemHeightDock(bool enable);

signals:
    void currentChanged(FwPrimitive* previous, FwPrimitive* current);
    void itemTriggered(FwPrimitive* item);

protected:
    void keyPressEvent(FwKeyPressEvent* keyEvent);

    void geometryChangedEvent(const QRect &oldRect, QRect &rect);
    void childrenRectChangedEvent(bool posChanged, bool sizeChanged);

    bool needInitLayout;

    void updateCurrent();

    virtual void itemAddedEvent(FwPrimitive* item);

private:
    QList<FwPrimitive*> m_items;
    FwPrimitive* m_current;
    bool m_currentDirty;
    FwPrimitive* m_previous;
    int m_startItemsChanged;

    FwRectPrimitive* m_highlight;

    FwMLObject* m_itemTemplate;
    FwColor m_itemColor;
    FwColor m_currentItemColor;

    FwItemLayout* m_layout;

    bool m_itemWidthDock;
    bool m_itemHeightDock;
};

FwItemLayout* FwItemView::layout() const
{
    return m_layout;
}

QList<FwPrimitive*> FwItemView::items() const
{
    return m_items;
}

FwPrimitive* FwItemView::current() const
{
    return m_current;
}

FwPrimitive* FwItemView::previous() const
{
    return m_previous;
}

FwMLObject* FwItemView::itemTemplate() const
{
    return m_itemTemplate;
}

FwColor FwItemView::itemColor() const
{
    return m_itemColor;
}

FwColor FwItemView::currentItemColor() const
{
    return m_currentItemColor;
}

FwRectPrimitive* FwItemView::highlight() const
{
    return m_highlight;
}

void FwItemView::prepareItemsChanged()
{
    ++m_startItemsChanged;
}

void FwItemView::clear()
{
    setItems(QList<FwPrimitive*>());
}

#endif // FIREWORKS_ITEMVIEW_H
