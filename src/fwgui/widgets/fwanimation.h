#ifndef FIREWORKS_ANIMATION_H
#define FIREWORKS_ANIMATION_H

#include <QtCore/qabstractanimation.h>

class FwMLObject;

class FwAnimation : public QAbstractAnimation
{
    Q_OBJECT
    typedef QAbstractAnimation BaseClass;

public:
    FwAnimation(QObject* parent = 0);

    void apply(FwMLObject* object);
};

#endif // FIREWORKS_ANIMATION_H
