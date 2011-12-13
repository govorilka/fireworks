#include <QtCore/QCoreApplication>
#include <QtCore/QTime>
#include <QtCore/qdebug.h>

#include "fwcore/fwmldocument.h"
#include "fw/scheduler.hpp"

class OwnTask : public Fw::Scheduler::Task
{
    typedef Fw::Scheduler::Task BaseClass;
    virtual void run();

public:
    explicit OwnTask(QObject* parent = 0, const QByteArray& name = QByteArray());
};

class OwnNetworkTask : public Fw::Scheduler::NetworkTask
{
    typedef Fw::Scheduler::NetworkTask BaseClass;

public:
    virtual bool replyMasterProcessed(QNetworkReply* reply);
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Fw::Scheduler scheduler;

    OwnTask* task = new OwnTask(0, "OwnTask");
    task->setInterval(15*1000);
    //scheduler.addTask(task);

    OwnNetworkTask* networkTask = new OwnNetworkTask();
    networkTask->setInterval(3*1000);
    networkTask->setUrl(QUrl("http://www.google.ru/"));
    scheduler.addTask(networkTask);

    FwMLObject config;
    config.parseFile("scheduler.fwml");

    scheduler.loadData(&config);

    scheduler.start();
    scheduler.startAllTasks();

    return app.exec();
}

void OwnTask::run()
{
    qDebug() << "OwnTask::run: " << QTime::currentTime();
}


OwnTask::OwnTask(QObject* parent, const QByteArray& name)
    :
    BaseClass(parent, name)
{
}

bool OwnNetworkTask::replyMasterProcessed(QNetworkReply* reply)
{
    qDebug() << reply->readAll();

    return true;
}
