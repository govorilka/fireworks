#include <QtCore/qcoreevent.h>

#include "fwgui/fwguievent.h"

#include "fwgui/primitives/fwstringprimitive.h"

#include "fwdigitinputwidget.h"

FwDigitInputWidget::FwDigitInputWidget(const QByteArray& name, FwPrimitiveGroup* parent) :
    BaseClass(name, parent),
    m_inputTimer(0),
    m_digitsLabel(new FwStringPrimitive("text", this)),
    m_value(0),
    m_digitsCount(0),
    m_maxValue(-1)
{
    m_digitsLabel->setString("00");
    setVisibleTime(2000); //default value - 2 sec
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
    QString text = event->text();
    if(text.isEmpty() || !text.at(0).isDigit())
    {
        BaseClass::keyPressEvent(event);
        return;
    }

    resetInputTimer();

    QString value;
    if(m_digitsCount)
    {
        value = m_digitsLabel->string();
    }
    value += text.at(0);

    bool bOk = false;
    int intValue = value.toInt(&bOk);
    if(bOk && maxValueCheck(intValue))
    {
        ++m_digitsCount;
        m_digitsLabel->setString(value);
        if(value.length() >= maxStringLength())
        {
            inputFinished();
        }
    }

    event->accept();
}

void FwDigitInputWidget::inputFinished()
{
    killInputTimer();
    m_digitsCount = 0;
    setValue(m_digitsLabel->string().toInt());
}

void FwDigitInputWidget::inputAbort()
{
    killInputTimer();
    m_digitsCount = 0;
    m_digitsLabel->setString(QString::number(m_value));
}

void FwDigitInputWidget::setValue(int value)
{
    if(!m_digitsCount)
    {
        if(m_value != value && maxValueCheck(value))
        {
            m_value = value;
            valueChangedEvent(m_value);
        }
        m_digitsLabel->setString(QString::number(m_value));
    }
}

void FwDigitInputWidget::valueChangedEvent(int value)
{
    emit valueChanged(value);
}

void FwDigitInputWidget::hideEvent(FwHideEvent* event)
{
    inputAbort();
    BaseClass::hideEvent(event);
}

void FwDigitInputWidget::setMaxValue(int maxValue)
{
    if(maxValue >= -1 && m_maxValue != maxValue)
    {
        m_maxValue = maxValue;
        if(m_maxValue != -1 && m_value > m_maxValue)
        {
            setValue(m_maxValue);
        }
    }
}
