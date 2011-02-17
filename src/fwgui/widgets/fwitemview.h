#ifndef FIREWORKS_ITEMVIEW_H
#define FIREWORKS_ITEMVIEW_H

#include <QtCore/qlist.h>
#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>

#include "fwgui/widgets/fwwidget.h"

class FwItemView;
class FwItemLayout;
class FwPrimitive;
class FwStringPrimitive;

typedef FwItemLayout*(FwLayoutConstructor)(FwItemView* view);

class FwItemView : public FwWidget
{
    Q_OBJECT
    typedef FwWidget BaseClass;

public:
    FwItemView(const QByteArray& name, FwPrimitiveGroup* parent);
    ~FwItemView();

    inline QList<FwPrimitive*> items() const;

    void addItem(FwPrimitive* primitive, const QVariant& data = QVariant());
    FwStringPrimitive* addItem(const QString& text, const QVariant& data = QVariant());

    inline FwPrimitive* current() const;
    void setCurrent(FwPrimitive* primitive);

    inline FwItemLayout* layout() const;
    void setLayout(FwItemLayout* layout);

    inline FwMLObject* itemTemplate() const;
    void setItemTemplate(FwMLObject* itemTemplate);

    void apply(FwMLObject *object);

    static bool addLayoutClass(const QByteArray& className, FwLayoutConstructor* constructor);
    FwItemLayout* createLayout(const QByteArray& className);

protected:
    void geometryChanged(const QRect &oldRect, QRect &rect);
    void invalidateChildrenRect();

private:
    QList<FwPrimitive*> m_items;
    FwPrimitive* m_current;
    FwMLObject* m_itemTemplate;

    FwItemLayout* m_layout;

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

FwMLObject* FwItemView::itemTemplate() const
{
    return m_itemTemplate;
}

#endif // FIREWORKS_ITEMVIEW_H
