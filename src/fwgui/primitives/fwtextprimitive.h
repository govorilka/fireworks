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

    inline bool isFixedHeightEnable() const;
    void setFixedHeight(bool enable);

protected:
    void geometryChangedEvent(const QRect &oldRect, QRect &rect);

    void paint(FwPainter *painter, const QRect &clipRect);

    inline void updateText();
    void updateTextLayout(QRect& rect);

private:
    QString m_text;
    FwFont m_font;
    QVector<FwTextString> m_strings;
    bool m_fixedHeight;
};

QString FwTextPrimitive::text() const
{
    return m_text;
}

FwFont FwTextPrimitive::font() const
{
    return m_font;
}

void FwTextPrimitive::updateText()
{
    prepareGeometryChanged();
    QRect newRect = geometry()->rect();
    updateTextLayout(newRect);
    setSize(newRect.size());
    update();
}

bool FwTextPrimitive::isFixedHeightEnable() const
{
    return m_fixedHeight;
}

#endif //FIREWORKS_GRAPHICSTEXTITEM_H
