#ifndef FIREWORKS_GUIFACTORY_H
#define FIREWORKS_GUIFACTORY_H

#include "fireworks.h"

class FwRectPrimitive;
class FwMLObject;

class FIREWORKSSHARED_EXPORT FwGuiFactory
{
public:
    static FwRectPrimitive* createRectPrimitive(const QByteArray& className, const QByteArray& name, FwPrimitiveGroup* parent);

};

#endif // FIREWORKS_GUIFACTORY_H
