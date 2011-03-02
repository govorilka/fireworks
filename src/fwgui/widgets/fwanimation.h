#ifndef FIREWORKS_ANIMATION_H
#define FIREWORKS_ANIMATION_H

#include <QtCore/qvariantanimation.h>

class FwMLObject;

class FwAnimation : public QVariantAnimation
{
    Q_OBJECT
    typedef QVariantAnimation BaseClass;

public:
    FwAnimation(QObject* parent = 0);

    void apply(FwMLObject* object);

    static QEasingCurve::Type nameToCurveType(const QByteArray& name, bool* bOk = 0);

    inline bool isRunning() const;
};

bool FwAnimation::isRunning() const
{
    return state() == QAbstractAnimation::Running;
}

#endif // FIREWORKS_ANIMATION_H
