#ifndef FIREWORKS_IMAGELIBRARY_H
#define FIREWORKS_IMAGELIBRARY_H

#include<QtCore/qhash.h>

#include "fwcore/fwcppobject.h"

#include "fwgui/fwpixmap.h"

class FwGraphicsView;

class FwImageLibrary : public FwCPPObject
{
    typedef FwCPPObject BaseClass;

public:
    FwImageLibrary(const QByteArray& name, FwGraphicsView* view);

    void apply(FwMLObject *object);

    FwPixmap icon(const QByteArray& name);

private:
    QHash<QByteArray, FwPixmapDescription> m_icons;
    FwGraphicsView* m_view;
};

#endif // FIREWORKS_IMAGELIBRARY_H
