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
        types.insert("inoutquart", QEasingCurve::InOutQuart);
        types.insert("inquad", QEasingCurve::InQuad);
        types.insert("outquad", QEasingCurve::OutQuad);
        types.insert("outinquad", QEasingCurve::OutInQuad);
        types.insert("inoutquad", QEasingCurve::InOutQuad);
        types.insert("outinquad", QEasingCurve::OutInQuad);
        types.insert("incubic", QEasingCurve::InCubic);
        types.insert("inoutcubic", QEasingCurve::InOutCubic);
        types.insert("outincubic", QEasingCurve::OutInCubic);
        types.insert("inquart", QEasingCurve::InQuart);

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
