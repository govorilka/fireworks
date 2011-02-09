#ifndef FIREWORKS_GRAPHICSSTRINGITEM_H
#define FIREWORKS_GRAPHICSSTRINGITEM_H

#include "fwprimitives/fwlineprimitive.h"

#include "fwcore/fwcolor.h"

#include "fwtypography/fwfont.h"

class FwStringPrimitive;
typedef FwStringPrimitive* FwStringPrimitivePtr;

class FwStringPrimitive : public FwLinePrimitive
{
    typedef FwLinePrimitive BaseClass;

public:
    FwStringPrimitive(const QByteArray& name, FwPrimitiveGroup* parent);

    inline QString string() const;
    void setString(const QString& string);

    inline FwFont font() const;
    void setFont(const FwFont& font);

    inline bool isShadowEnable() const;
    void setShadowEnabled(bool enable);

    inline FwColor shadowColor() const;
    void setShadowColor(const FwColor& color);

    inline bool isFixedSize() const;
    void setFixedSize(bool enable, const QString& mask = QString());

    void apply(FwMLObject *object);

protected:
    QRect updateGeometry(const QRect& rect);

    void updateStringRect(const QString &string);

    void paint(FwPainter *painter, const QRect &clipRect);

    void penChangedEvent(FwPen* pen);

private:
    QString m_string;
    QByteArray m_utf8String;
    FwFont m_font;
    QPoint m_textPos;
    bool m_shadow;
    FwColor m_shadowColor;
    bool m_fixedSize;
    QString m_mask;
    QRect m_stringRect;
};

#include "fwprimitives/fwstringprimitive_inl.h"

#endif // FIREWORKS_GRAPHICSSTRINGITEM_H
