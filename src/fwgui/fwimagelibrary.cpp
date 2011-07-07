#include "fwcore/fwml.h"

#include "fwimagelibrary.h"

#include "fwgui/fwgraphicsview.h"

#include "fwgui/primitives/fwprimitive.h"


FwImageLibrary::FwImageLibrary(const QByteArray& name, FwGraphicsView* view) :
    BaseClass(name),
    m_view(view)
{
}

void FwImageLibrary::apply(FwMLObject *object)
{
    QHash<QByteArray, FwMLNode*> iconsNode = object->attributes();
    foreach(FwMLNode* item, iconsNode)
    {
        FwPixmapDescription iconDescription;
        if(iconDescription.apply(item))
        {
            m_icons.insert(item->name(), iconDescription);
        }
    }
}

FwPixmap FwImageLibrary::icon(const QByteArray &name)
{
    if(!m_icons.isEmpty())
    {
        FwPixmapDescription desc = m_icons.value(name, FwPixmapDescription());
        if(!desc.isNull())
        {
            return m_view->pixmap(desc);
        }
    }
    return FwPixmap();
}
