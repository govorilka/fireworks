#include "fwcore/fwml.h"

#include "fwgui/widgets/fwpixmapbar.h"

#include "fwguifactory.h"

FwRectPrimitive* FwGuiFactory::createRectPrimitive(const QByteArray& className, const QByteArray& name, FwPrimitiveGroup* parent)
{
    if(!className.isEmpty())
    {
        QByteArray lowerClassName = className.toLower();
        if(lowerClassName == "fwgui.primitives.rect")
        {
            return new FwRectPrimitive(name, parent);
        }
        else if(lowerClassName == "fwgui.primitives.pixmapbar")
        {
            return new FwPixmapBar(name, parent);
        }
    }
    return 0;
}
