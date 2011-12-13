#ifndef FIREWORKS_SCHEDULER_INL_H
#define FIREWORKS_SCHEDULER_INL_H

#include "fw/scheduler.hpp"

bool Fw::Scheduler::startTask(int taskId)
{
    return startTask(m_tasks.value(taskId, 0));
}

bool Fw::Scheduler::stopTask(int taskId)
{
    return stopTask(m_tasks.value(taskId, 0));
}

QString Fw::Scheduler::lastError() const
{
    return errorString;
}

////////////////////////////////////////////////////////////////////

int Fw::Scheduler::Task::interval() const
{
    return m_interval;
}

bool Fw::Scheduler::Task::isRunningOnStart() const
{
    return m_runOnStart;
}

////////////////////////////////////////////////////////////////////

QString Fw::Scheduler::SystemTask::command() const
{
    return m_command;
}

////////////////////////////////////////////////////////////////////

QUrl Fw::Scheduler::NetworkTask::url() const
{
    return m_url;
}

////////////////////////////////////////////////////////////////////

int Fw::Scheduler::TaskEvent::status() const
{
    return m_status;
}

#endif // FIREWORKS_SCHEDULER_INL_H

