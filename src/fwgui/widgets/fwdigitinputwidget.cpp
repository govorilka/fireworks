#include <QtCore/qcoreevent.h>

#include "fwgui/fwguievent.h"

#include "fwgui/primitives/fwstringprimitive.h"

#include "fwdigitinputwidget.h"

FwDigitInputWidget::FwDigitInputWidget(const QByteArray& name, FwPrimitiveGroup* parent) :
    BaseClass(name, parent),
    m_inputTimer(0),
    m_digitsLabel(new FwStringPrimitive("text", this)),
    m_value(0),
    m_digitsCount(0)
{
    m_digitsLabel->setString("00");
}


void FwDigitInputWidget::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_inputTimer)
    {
        inputFinished();
        return;
    }

    BaseClass::timerEvent(event);
}

void FwDigitInputWidget::keyPressEvent(FwKeyPressEvent *event)
{
    int digit = event->digit();
    if(digit == -1)
    {
        BaseClass::keyPressEvent(event);
        return;
    }

    resetInputTimer();

    m_value = m_digitsCount ? m_value * 10 + digit : digit;
    ++m_digitsCount;

    m_digitsLabel->setString(QString::number(m_value));

    event->accept();
}

void FwDigitInputWidget::inputFinished()
{
    killInputTimer();
    m_digitsCount = 0;
    emit valueChanged(m_value);
}

void FwDigitInputWidget::inputAbort()
{
    killInputTimer();
    m_digitsCount = 0;
}

void FwDigitInputWidget::hideEvent(FwHideEvent* event)
{
    inputAbort();
    BaseClass::hideEvent(event);
}
