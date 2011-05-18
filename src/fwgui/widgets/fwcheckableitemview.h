#ifndef FWCHECKABLEITEMVIEW_H
#define FWCHECKABLEITEMVIEW_H

#include "fwgui/widgets/fwitemview.h"

class FwCheckableItemView : public FwItemView
{
    Q_OBJECT
    typedef FwItemView BaseClass;
public:
    FwCheckableItemView(const QByteArray& name, FwPrimitiveGroup* parent);
};

#endif // FWCHECKABLEITEMVIEW_H
