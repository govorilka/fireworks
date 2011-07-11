#ifndef FIREWORKS_DIGITINPUT_WIDGET_H
#define FIREWORKS_DIGITINPUT_WIDGET_H

#include "fireworks.h"

#include "fwgui/widgets/fwwidget.h"

class FwStringPrimitive;

class FIREWORKSSHARED_EXPORT FwDigitInputWidget : public FwWidget
{
    Q_OBJECT
    typedef FwWidget BaseClass;

public:
    FwDigitInputWidget(const QByteArray& name, FwPrimitiveGroup* parent);

signals:
    void valueChanged(int value);

protected:
    void timerEvent(QTimerEvent *event);

    void keyPressEvent(FwKeyPressEvent *event);
    void hideEvent(FwHideEvent *event);

    void inputFinished();
    void inputAbort();

    inline void resetInputTimer();
    inline void killInputTimer();

private:
    int m_inputTimer;

    int m_value;
    int m_digitsCount;

    FwStringPrimitive* m_digitsLabel;
};

void FwDigitInputWidget::killInputTimer()
{
    if(m_inputTimer)
    {
        killTimer(m_inputTimer);
        m_inputTimer = 0;
    }
}

void FwDigitInputWidget::resetInputTimer()
{
    killInputTimer();
    m_inputTimer = startTimer(1000);
}

#endif // FIREWORKS_DIGITINPUT_WIDGET_H
