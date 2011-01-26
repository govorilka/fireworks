#ifndef FIREWORKS_GRAPHICSTEXTITEM_H
#define FIREWORKS_GRAPHICSTEXTITEM_H

#include <QtCore/qvector.h>

#include "fwprimitives/fwrectprimitive.h"

#include "fwtypography/fwfont.h"
#include "fwtypography/fwtextstring.h"

#include "fwgui/fwpen.h"

class FwTextPrimitive : public FwRectPrimitive
{
    typedef FwRectPrimitive BaseClass;

public:
    FwTextPrimitive(FwPrimitiveGroup* parent);

    inline QString text() const;
    void setText(const QString& text);

    inline FwFont font() const;
    void setFont(const FwFont& font);

    inline FwPenPtr pen() const;
    void setPen(const FwPenPtr& pen);

    void apply(FwMLObject *object);

protected:
    QRect updateGeometry(const QRect &rect);

    void paint(FwPainter *painter, const QRect &clipRect);

private:
    QString m_text;
    FwFont m_font;
    FwPenPtr m_pen;
    QVector<FwTextString> m_strings;
};

QString FwTextPrimitive::text() const
{
    return m_text;
}

FwFont FwTextPrimitive::font() const
{
    return m_font;
}

FwPenPtr FwTextPrimitive::pen() const
{
    return m_pen;
}

#endif //FIREWORKS_GRAPHICSTEXTITEM_H
