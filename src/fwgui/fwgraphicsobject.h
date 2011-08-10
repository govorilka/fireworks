#ifndef FIREWORKS_GRAPHICSOBJECT_H
#define FIREWORKS_GRAPHICSOBJECT_H

#include <QtCore/qobject.h>

#include "fwgui/primitives/fwprimitivegroup.h"

class QResizeEvent;
class QPropertyAnimation;

class FwResizeEvent;
class FwKeyPressEvent;

class FwObjectGraphicsItem;

class FwResult;

class FIREWORKSSHARED_EXPORT FwGraphicsObject
{
    typedef FwPrimitiveGroup BaseClass;

public:
    friend class FwScene;
    friend class FwWidget;
    friend class FwPrimitive;
};

#include "fwgui/fwgraphicsobject_inl.h"

#endif // FIREWORKS_GRAPHICSOBJECT_H
