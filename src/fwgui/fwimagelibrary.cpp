#include "fwcore/fwml.h"

#include "fwimagelibrary.h"

#include "fwgui/fwgraphicsview.h"

#include "fwgui/primitives/fwprimitive.h"


FwImageLibrary::FwImageLibrary(const QByteArray& name, FwGraphicsView* view) :
    BaseClass(name),
    m_view(view)
{
}

bool FwImageLibrary::loadData(FwMLObject *object)
{
    QHash<QByteArray, FwMLNode*> iconsNode = object->attributes();
    foreach(FwMLNode* item, iconsNode)
    {
        FwPixmapDescription iconDescription;
        if(iconDescription.apply(item))
        {
            m_descriptions.insert(item->name(), iconDescription);
        }
    }
    return true;
}

FwPixmap FwImageLibrary::image(const QByteArray &name)
{
    FwPixmap pixmap = m_images.value(name, FwPixmap());
    if(!pixmap.isNull())
    {
        return pixmap;
    }

    FwPixmapDescription desc = m_descriptions.value(name, FwPixmapDescription());
    if(!desc.isNull())
    {
        return m_view->pixmap(desc);
    }

    return FwPixmap();
}
