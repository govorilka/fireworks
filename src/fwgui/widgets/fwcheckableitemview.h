#ifndef FIREWORKS_CHECKABLEITEMVIEW_H
#define FIREWORKS_CHECKABLEITEMVIEW_H

#include "fwgui/widgets/fwitemview.h"

#include "fwgui/fwpixmap.h"

class FwCheckableItem;
class FwPixmapPrimitive;

class FwCheckableItemView : public FwItemView
{
    Q_OBJECT
    typedef FwItemView BaseClass;

public:
    FwCheckableItemView(const QByteArray& name, FwPrimitiveGroup* parent);

    bool check(FwPrimitive* item) const;
    void setCheck(FwPrimitive* item, bool value);

    inline FwPixmap pixmapCheckOn() const;
    void setPixmapCheckOn(const FwPixmap& pixmap);

    inline FwPixmap pixmapCheckOff() const;
    void setPixmapCheckOff(const FwPixmap& pixmap);

    void apply(FwMLObject *object);

protected:
    void itemAddedEvent(FwPrimitive* item);
    void itemTriggered(FwPrimitive* item);

private:
    QHash<FwPrimitive*, FwCheckableItem*> m_checkableItems;
    FwPixmap m_pixmapCheckOn;
    FwPixmap m_pixmapCheckOff;
    FwMLObject* m_checkboxTemplate;
};

FwPixmap FwCheckableItemView::pixmapCheckOn() const
{
    return m_pixmapCheckOn;
}

FwPixmap FwCheckableItemView::pixmapCheckOff() const
{
    return m_pixmapCheckOff;
}

///////////////////////////////////////////////////////////////////////////////

class FwCheckableItem
{
public:
    friend class FwCheckableItemView;

    FwCheckableItem(FwPrimitive* item, FwCheckableItemView* parent);

private:
    FwPrimitive* m_item;
    FwCheckableItemView* m_parent;
    FwPixmapPrimitive* m_checkBox;

    bool m_check;
};

#endif // FIREWORKS_CHECKABLEITEMVIEW_H
