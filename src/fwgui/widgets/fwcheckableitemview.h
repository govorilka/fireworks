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

protected:
    void itemTriggered(FwPrimitive* item);

private:
    QList<FwCheckableItem*> m_checkableItems;
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

QList<FwCheckableItem*> FwCheckableItemView::checkableItems() const
{
    return m_checkableItems;
}

///////////////////////////////////////////////////////////////////////////////

class FwCheckableItem : public FwPrimitiveGroup
{
    typedef FwPrimitiveGroup BaseClass;

public:
    FwCheckableItem(FwCheckableItemView* parent);

    inline bool isChecked() const;
    void setCheck(bool value);

    inline void toggleCheck();

    inline FwStringPrimitive* caption() const;

    void updatePixmaps();

protected:
    void updateChildrenRect();

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

void FwCheckableItem::toggleCheck()
{
    setCheck(!isChecked());
}

FwStringPrimitive* FwCheckableItem::caption() const
{
    return m_caption;
}

#endif // FIREWORKS_CHECKABLEITEMVIEW_H
