#ifndef FIREWORKS_TEXTVIEW_H
#define FIREWORKS_TEXTVIEW_H

#include "fwgui/widgets/fwwidget.h"

class FwTextPrimitive;

class FwTextView : public FwWidget
{
    Q_OBJECT
    typedef FwWidget BaseClass;

public:
    FwTextView(const QByteArray& name, FwPrimitiveGroup* parent);
    ~FwTextView();

    inline FwTextPrimitive* textPrimitive() const;

private:
    FwTextPrimitive* m_text;
};

FwTextPrimitive* FwTextView::textPrimitive() const
{
    return m_text;
}

#endif // FIREWORKS_TEXTVIEW_H
