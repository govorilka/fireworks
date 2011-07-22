#ifndef FIREWORKS_DIGITINPUT_WIDGET_H
#define FIREWORKS_DIGITINPUT_WIDGET_H

#include <QtCore/qmath.h>

#include "fireworks.h"

#include "fwgui/widgets/fwwidget.h"

class FwStringPrimitive;

class FIREWORKSSHARED_EXPORT FwDigitInputWidget : public FwWidget
{
    Q_OBJECT
    typedef FwWidget BaseClass;

public:
    FwDigitInputWidget(const QByteArray& name, FwPrimitiveGroup* parent);

    inline int value() const;

    inline int maxValue() const;
    void setMaxValue(int maxValue);

public slots:
    void setValue(int value);

signals:
    void valueChanged(int value);

protected:
    void timerEvent(QTimerEvent *event);

    void keyPressEvent(FwKeyPressEvent *event);
    void hideEvent(FwHideEvent *event);
    virtual void valueChangedEvent(int value);

    void inputFinished();
    void inputAbort();

    inline void resetInputTimer();
    inline void killInputTimer();

    inline bool maxValueCheck(int value) const;
    inline int maxStringLength() const;

private:
    int m_inputTimer;

    int m_value;
    int m_maxValue;

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
    m_inputTimer = startTimer(visibleTime() == 0 ? 1000 : qMin(visibleTime(), 1000));
    startVisibleTimer();
}

int FwDigitInputWidget::value() const
{
    return m_value;
}

int FwDigitInputWidget::maxValue() const
{
    return m_maxValue;
}

bool FwDigitInputWidget::maxValueCheck(int value) const
{
    return m_maxValue == -1 || value <= m_maxValue;
}
int FwDigitInputWidget::maxStringLength() const
{
    return m_maxValue == -1 ? 10 : qLn(m_maxValue) / qLn(10) + 1;
}

#endif // FIREWORKS_DIGITINPUT_WIDGET_H
