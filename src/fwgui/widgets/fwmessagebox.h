#ifndef FIREWORKS_MESSAGEBOX_H
#define FIREWORKS_MESSAGEBOX_H

#include "fwgui/widgets/fwwidget.h"

#include "fwgui/primitives/fwstringprimitive.h"

#include "fwutils/fwrequest.h"

class FIREWORKSSHARED_EXPORT FwMessageBox : public FwWidget
{
    Q_OBJECT
    typedef FwWidget BaseClass;

public:
    explicit FwMessageBox(const QByteArray& name, FwPrimitiveGroup* parent);

    void apply(FwMLObject *object);

    inline FwStringPrimitive* caption() const;

    inline void setRequest(const FwRequest& request);

    bool accept(int result);
    bool acceptKey(int key);

protected:
    void keyPressEvent(FwKeyPressEvent *event);

private:
    FwStringPrimitive* m_caption;
    FwRequest m_request;
};

FwStringPrimitive* FwMessageBox::caption() const
{
    return m_caption;
}


void FwMessageBox::setRequest(const FwRequest& request)
{
    m_request = request;
}

#endif // FIREWORKS_MESSAGEBOX_H
