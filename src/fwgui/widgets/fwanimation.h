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

    virtual void apply(FwMLObject* object);

    static QEasingCurve::Type nameToCurveType(const QByteArray& name, bool* bOk = 0);

    inline bool isRunning() const;

    inline void restoreCurve();
    inline void resetCurve();

    inline bool isSmooth() const;
    inline void setSmooth(bool smooth);

private:
    QEasingCurve m_curve;
    bool m_smooth;
};

bool FwAnimation::isRunning() const
{
    return state() == QAbstractAnimation::Running;
}

void FwAnimation::restoreCurve()
{
    if(m_smooth && m_curve.type() != QEasingCurve::Linear)
    {
        setEasingCurve(m_curve);
    }
}

void FwAnimation::resetCurve()
{
    if(m_smooth)
    {
        QEasingCurve curve = easingCurve();
        if(curve.type() != QEasingCurve::Linear)
        {
            m_curve = curve;
            setEasingCurve(QEasingCurve::Linear);
        }
    }
}

bool FwAnimation::isSmooth() const
{
    return m_smooth;
}

void FwAnimation::setSmooth(bool smooth)
{
    m_smooth = smooth;
}


#endif // FIREWORKS_ANIMATION_H
