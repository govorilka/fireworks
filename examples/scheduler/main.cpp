#include <QtCore/QCoreApplication>
#include <QtCore/QTime>
#include <QtCore/qdebug.h>

#include "fw/scheduler.hpp"

class OwnTask : public Fw::Scheduler::Task
{
virtual void run();
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Fw::Scheduler scheduler;

    OwnTask* task = new OwnTask();
    task->setInterval(1.5*1000);
    scheduler.addTask(task);
    scheduler.start();
    scheduler.startAllTasks();

    return app.exec();
}

void OwnTask::run()
{
    qDebug() << "OwnTask::run: " << QTime::currentTime();
}
