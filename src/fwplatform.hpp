#ifndef FIREWORKS_PLATFORM_HPP
#define FIREWORKS_PLATFORM_HPP

#include "fireworks.h"
#include "fwcore/fwcppobject.h"

namespace Fw
{
    class Platform;
}

class FIREWORKSSHARED_EXPORT Fw::Platform : public QObject, public FwCPPObject
{
    Q_OBJECT
    typedef FwCPPObject BaseClass;

public:
    explicit Platform(QObject* parent);
    virtual ~Platform();

    virtual bool loadData(FwMLObject* object);
};

#endif //FIREWORKS_PLATFORM_HPP
