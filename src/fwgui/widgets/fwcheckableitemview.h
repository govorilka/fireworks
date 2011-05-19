#ifndef FIREWORKS_CHECKABLEITEMVIEW_H
#define FIREWORKS_CHECKABLEITEMVIEW_H

#include "fwgui/primitives/fwpixmapprimitive.h"

#include "fwgui/widgets/fwitemview.h"

#include "fwgui/primitives/fwprimitivegroup.h"

class FwCheckableItem;
class FwPixmapPrimitive;

const FwPixmap m_pixmapCheckOn;
const FwPixmap m_pixmapCheckOff;


class FwCheckableItemView : public FwItemView
{
    Q_OBJECT
    typedef FwItemView BaseClass;

public:
    FwCheckableItemView(const QByteArray& name, FwPrimitiveGroup* parent);


    bool check(FwPrimitive* item) const;
    void setCheck(FwPrimitive* item, bool value);

protected:
    void itemAddedEvent(FwPrimitive* item);


private:
    QHash<FwPrimitive*, FwCheckableItem*> m_checkableItems;

};

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
