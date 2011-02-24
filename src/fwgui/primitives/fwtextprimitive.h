#ifndef FIREWORKS_GRAPHICSTEXTITEM_H
#define FIREWORKS_GRAPHICSTEXTITEM_H

#include <QtCore/qvector.h>

#include "fwgui/primitives/fwrectprimitive.h"

#include "fwtypography/fwfont.h"
#include "fwtypography/fwtextstring.h"

class FwPen;

class FwTextPrimitive : public FwRectPrimitive
{
    typedef FwRectPrimitive BaseClass;

public:
    FwTextPrimitive(const QByteArray& name, FwPrimitiveGroup* parent);
    ~FwTextPrimitive();

    inline QString text() const;
    void setText(const QString& text);

    inline FwFont font() const;
    void setFont(const FwFont& font);

    void apply(FwMLObject *object);

protected:
    void geometryChanged(const QRect &oldRect, QRect &rect);

    void paint(FwPainter *painter, const QRect &clipRect);

private:
    QString m_text;
    FwFont m_font;
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

#endif //FIREWORKS_GRAPHICSTEXTITEM_H
