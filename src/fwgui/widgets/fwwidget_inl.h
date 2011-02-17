#ifndef FIREWORKS_WIDGET_INL_H
#define FIREWORKS_WIDGET_INL_H

#include "fwwidget.h"

int FwWidget::visibleTime() const
{
    return m_visibleTime;
}

void FwWidget::setVisibleTime(int msec)
{
    if(m_visibleTime != msec)
    {
        (m_visibleTime = msec) ? startVisibleTimer() : stopVisibleTimer();
    }
}

#endif // FWWIDGET_INL_H
