#include "fwanimation.h"

#include "fwcore/fwml.h"

FwAnimation::FwAnimation(QObject* parent) :
    BaseClass(parent)
{
    setDuration(250);
    setEasingCurve(QEasingCurve::Linear);
}

void FwAnimation::apply(FwMLObject* object)
{
    FwMLNode* durationNode = object->attribute("duration");
    if(durationNode)
    {
        bool bOk = false;
        int duration = durationNode->toInt(&bOk);
        if(bOk)
        {
            setDuration(duration);
        }
    }

    FwMLString* curveNode = object->attribute("curve")->cast<FwMLString>();
    if(curveNode)
    {
        bool bOk = false;
        QEasingCurve::Type curveType = nameToCurveType(curveNode->value(), &bOk);
        if(bOk)
        {
            setEasingCurve(curveType);
        }
    }
}

QEasingCurve::Type FwAnimation::nameToCurveType(const QByteArray& name, bool* bOk)
{
    if(!name.isEmpty())
    {
        QHash<QByteArray, QEasingCurve::Type> types;
        types.insert("linear", QEasingCurve::Linear);
        types.insert("outbounce", QEasingCurve::OutBounce);
        types.insert("outcubic", QEasingCurve::OutCubic);

        QByteArray lowerName = name.toLower();
        if(types.contains(lowerName))
        {
            if(bOk)
            {
                (*bOk) = true;
            }
            return types.value(lowerName);
        }

    }

    if(bOk)
    {
        (*bOk) = false;
    }
    return QEasingCurve::Linear;
}
