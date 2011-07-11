#include "fwcore/fwml.h"

#include "fwgui/primitives/fwrectprimitive.h"
#include "fwgui/primitives/fwpixmapbarprimitive.h"

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
            return new FwPixmapBarPrimtive(name, parent);
        }
    }
    return 0;
}
