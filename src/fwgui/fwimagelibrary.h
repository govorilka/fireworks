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

    bool loadData(FwMLObject *object);

    FwPixmap image(const QByteArray& name);

private:
    QHash<QByteArray, FwPixmap> m_images;
    QHash<QByteArray, FwPixmapDescription> m_descriptions;
    FwGraphicsView* m_view;
};

#endif // FIREWORKS_IMAGELIBRARY_H
