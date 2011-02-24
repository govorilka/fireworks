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
    friend class FwItemLayout;

    FwItemView(const QByteArray& name, FwPrimitiveGroup* parent);
    ~FwItemView();

    inline QList<FwPrimitive*> items() const;

    void addItem(FwPrimitive* primitive, const QVariant& data = QVariant());
    FwStringPrimitive* addItem(const QString& text, const QVariant& data = QVariant());

    inline FwPrimitive* current() const;
    inline FwPrimitive* previous() const;
    void setCurrent(FwPrimitive* primitive);

    inline FwItemLayout* layout() const;
    void setLayout(FwItemLayout* layout);

    inline FwMLObject* itemTemplate() const;
    void setItemTemplate(FwMLObject* itemTemplate);

    void apply(FwMLObject *object);

    static bool addLayoutClass(const QByteArray& className, FwLayoutConstructor* constructor);
    FwItemLayout* createLayout(const QByteArray& className);

signals:
    void currentChanged(FwPrimitive* previous, FwPrimitive* current);

protected:
    void keyPressEvent(FwKeyPressEvent* keyEvent);

    void geometryChanged(const QRect &oldRect, QRect &rect);
    void invalidateChildrenRect();

    bool needInitLayout;

    void applyCurrentItem(FwPrimitive* primitive);

private:
    QList<FwPrimitive*> m_items;
    FwPrimitive* m_current;
    FwPrimitive* m_previous;

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

FwPrimitive* FwItemView::previous() const
{
    return m_previous;
}

FwMLObject* FwItemView::itemTemplate() const
{
    return m_itemTemplate;
}

#endif // FIREWORKS_ITEMVIEW_H
