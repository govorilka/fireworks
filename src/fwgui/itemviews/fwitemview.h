#ifndef FIREWORKS_ITEMVIEW_H
#define FIREWORKS_ITEMVIEW_H

#include "fwgui/fwwidget.h"

class FwItemView : public FwWidget
{
    Q_OBJECT
    typedef FwWidget BaseClass;

public:
    FwItemView(const QByteArray& name, FwPrimitiveGroup* parent);
};

#endif // FIREWORKS_ITEMVIEW_H
