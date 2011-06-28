#ifndef FIREWORKS_SCHEDULER_INL_H
#define FIREWORKS_SCHEDULER_INL_H

#include "fwscheduler.h"

bool FwScheduler::startTask(int taskId)
{
    return startTask(m_tasks.value(taskId, 0));
}

bool FwScheduler::stopTask(int taskId)
{
    return stopTask(m_tasks.value(taskId, 0));
}

QString FwScheduler::lastError() const
{
    return errorString;
}

////////////////////////////////////////////////////////////////////

int FwSchedulerTask::interval() const
{
    return m_interval;
}

bool FwSchedulerTask::isRunningOnStart() const
{
    return m_runOnStart;
}

////////////////////////////////////////////////////////////////////

QString FwSystemSchedulerTask::command() const
{
    return m_command;
}

////////////////////////////////////////////////////////////////////

QUrl FwNetworkSchedulerTask::url() const
{
    return m_url;
}

////////////////////////////////////////////////////////////////////

int FwSchedulerTaskEvent::status() const
{
    return m_status;
}

#endif // FIREWORKS_SCHEDULER_INL_H

