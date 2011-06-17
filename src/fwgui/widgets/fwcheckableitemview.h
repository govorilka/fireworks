#ifndef FIREWORKS_CHECKABLEITEMVIEW_H
#define FIREWORKS_CHECKABLEITEMVIEW_H

#include "fwgui/widgets/fwitemview.h"

#include "fwgui/fwpixmap.h"

class FwCheckableItem;
class FwPixmapPrimitive;

class FIREWORKSSHARED_EXPORT FwCheckableItemView : public FwItemView
{
    Q_OBJECT
    typedef FwItemView BaseClass;

public:
    friend class FwCheckableItem;

    FwCheckableItemView(const QByteArray& name, FwPrimitiveGroup* parent);

    inline FwPixmap pixmapCheckOn() const;
    void setPixmapCheckOn(const FwPixmap& pixmap);

    inline FwPixmap pixmapCheckOff() const;
    void setPixmapCheckOff(const FwPixmap& pixmap);

    void apply(FwMLObject *object);

    void addItem(FwCheckableItem* item);
    FwCheckableItem* addItem(const QString& caption, bool check, const QVariant& data = QVariant());

    inline QList<FwCheckableItem*> checkableItems() const;

    inline bool isExclusive() const;
    void setExclusive(bool enable);

private:
    QList<FwCheckableItem*> m_checkableItems;
    FwPixmap m_pixmapCheckOn;
    FwPixmap m_pixmapCheckOff;
    FwMLObject* m_checkboxTemplate;
    bool m_exclusive;
};

FwPixmap FwCheckableItemView::pixmapCheckOn() const
{
    return m_pixmapCheckOn;
}

FwPixmap FwCheckableItemView::pixmapCheckOff() const
{
    return m_pixmapCheckOff;
}

QList<FwCheckableItem*> FwCheckableItemView::checkableItems() const
{
    return m_checkableItems;
}

bool FwCheckableItemView::isExclusive() const
{
    return m_exclusive;
}

///////////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT FwCheckableItem : public FwPrimitiveGroup
{
    typedef FwPrimitiveGroup BaseClass;

public:
    FwCheckableItem(FwCheckableItemView* parent);

    inline bool isChecked() const;
    void setCheck(bool value);

    inline FwStringPrimitive* caption() const;

    void updatePixmaps();

protected:
    void updateChildrenRect();

    void currentChangedEvent(FwItemView *view, bool current);
    bool trigger();

private:
    FwCheckableItemView* m_parent;
    FwStringPrimitive* m_caption;
    FwPixmapPrimitive* m_checkBox;
    bool m_check;
};

bool FwCheckableItem::isChecked() const
{
    return m_check;
}

FwStringPrimitive* FwCheckableItem::caption() const
{
    return m_caption;
}

#endif // FIREWORKS_CHECKABLEITEMVIEW_H
