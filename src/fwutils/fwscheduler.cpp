#include <QtCore/qcoreapplication.h>
#include <QtCore/qprocess.h>
#include <QtCore/qdebug.h>

#include "fwscheduler.h"
#include "fireworks.h"

FwScheduler::FwScheduler(QObject *parent) :
    BaseClass(parent),
    networkManager(new FwSchedulerNetworkManager())
{
    networkManager->setParent(0);
    networkManager->moveToThread(this);
}

FwScheduler::~FwScheduler()
{
}

int FwScheduler::addTask(FwSchedulerTask* task)
{
    if(task->scheduler && task->scheduler != this)
    {
        return -1;
    }

    int id = m_tasks.key(task, -1);
    if(id == -1)
    {
        id = m_tasks.size();
        m_tasks.insert(id, task);

        task->setParent(0);
        task->moveToThread(this);

        FwNetworkSchedulerTask* networkTask = qobject_cast<FwNetworkSchedulerTask*>(task);
        if(networkTask)
        {
            networkTask->networkManager = networkManager;
        }
    }

    return id;
}

bool FwScheduler::startTask(FwSchedulerTask* task)
{
    if(task)
    {
        FwSchedulerTaskEvent* event = new FwSchedulerTaskEvent();
        event->setStatus(Fw::TS_Start);
        QCoreApplication::postEvent(task, event);
        return true;
    }
    return false;
}

bool FwScheduler::stopTask(FwSchedulerTask* task)
{
    if(task)
    {
        FwSchedulerTaskEvent* event = new FwSchedulerTaskEvent();
        event->setStatus(Fw::TS_Stop);
        QCoreApplication::postEvent(task, event);
        return true;
    }
    return false;
}

void FwScheduler::startAllTasks()
{
    foreach(FwSchedulerTask* task, m_tasks.values())
    {
        startTask(task);
    }
}

void FwScheduler::stopAllTasks()
{
    foreach(FwSchedulerTask* task, m_tasks.values())
    {
        stopTask(task);
    }
}

bool FwScheduler::postEvent(int taskId, QEvent* event)
{
    FwSchedulerTask* task = m_tasks.value(taskId, 0);
    if(task)
    {
        QCoreApplication::postEvent(task, event);
        return true;
    }
    return false;
}

QList<FwSchedulerTask*> FwScheduler::loadTasks(const QString& filename, QString* error)
{
    QFileInfo fInfo(filename);
    if(!fInfo.exists())
    {
        (*error) = QString("File %1 not found").arg(filename);
        return QList<FwSchedulerTask*>();
    }

    QFile tasksFile(filename);
    if(!tasksFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        (*error) = tasksFile.errorString();
        return QList<FwSchedulerTask*>();
    }

    QList<FwSchedulerTask*> tasks;

    int lineIndex = 0;
    while(!tasksFile.atEnd())
    {
        QString line = QString::fromUtf8(tasksFile.readLine());
        lineIndex++;

        int commentIndex = line.indexOf('#');
        if(commentIndex != -1)
        {
            line = line.left(commentIndex);
        }
        line = line.simplified();
        if(!line.isEmpty())
        {
            int separator = line.indexOf(' ');
            if(separator != -1)
            {
                int time = line.left(separator).toInt();
                if(time)
                {
                    QString command = line.right(line.length() - separator - 1).trimmed();
                    if(!command.isEmpty())
                    {
                        FwSystemSchedulerTask* task = new FwSystemSchedulerTask();
                        task->setCommand(command);
                        task->setRunOnStart(true);
                        task->setInterval(time * 1000);
                        tasks.append(task);
                        continue;
                    }
                }
            }

            (*error) = QString("Invalid file structure, line %1").arg(lineIndex);
            foreach(FwSchedulerTask* task, tasks)
            {
                task->deleteLater();
            }
            tasks.clear();
            return QList<FwSchedulerTask*>();
        }
    }

    return tasks;
}

void FwScheduler::run()
{
    BaseClass::run();
    release();
}

void FwScheduler::release()
{
    foreach(FwSchedulerTask* task, m_tasks.values())
    {
        task->scheduler = 0;
        task->moveToThread(QCoreApplication::instance()->thread());
        task->stop();
        task->deleteLater();
        task = 0;
    }
    m_tasks.clear();

    if(networkManager)
    {
        networkManager->deleteLater();
        networkManager = 0;
    }
}

////////////////////////////////////////////////////////////////////

FwSchedulerNetworkManager::FwSchedulerNetworkManager(QObject* parent) :
    BaseClass(parent)
{
    connect(this, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
}

QNetworkReply* FwSchedulerNetworkManager::get(FwNetworkSchedulerTask* task, const QUrl& url)
{
    QNetworkReply* reply = BaseClass::get(QNetworkRequest(url));
    if(reply)
    {
        replyCache.insert(qHash(reply), task);
        connect(reply, SIGNAL(destroyed(QObject*)), this, SLOT(replyDestroy(QObject*)));
    }
    return reply;
}

void FwSchedulerNetworkManager::replyFinished(QNetworkReply* reply)
{
    int hash = qHash(reply);
    if(replyCache.contains(hash))
    {
        QPointer<FwNetworkSchedulerTask> task = replyCache.value(hash, 0);
        if(task)
        {
            task->replyFinished(reply);
        }
        reply->deleteLater();
        reply = 0;
        replyCache.remove(hash);
    }
}

void FwSchedulerNetworkManager::replyDestroy(QObject* object)
{
    int hash = qHash(object);
    if(replyCache.contains(hash))
    {
        replyCache.remove(hash);
    }
}

////////////////////////////////////////////////////////////////////

FwSchedulerTask::FwSchedulerTask(QObject *parent) :
    QObject(parent),
    scheduler(0),
    m_timerId(0),
    m_status(Fw::TS_Stop),
    m_interval(0),
    m_runOnStart(true)
{
}

FwSchedulerTask::~FwSchedulerTask()
{
    stop();
    Q_ASSERT(scheduler);
}

void FwSchedulerTask::stop()
{
    if(m_status != Fw::TS_Stop)
    {
        m_status = Fw::TS_Stop;
        killTaskTimer();
    }
}

void FwSchedulerTask::start()
{
    if(m_status == Fw::TS_Stop)
    {
        m_status = Fw::TS_Start;
        if(m_runOnStart)
        {
            run();
        }
        startTaskTimer();
    }
}

void FwSchedulerTask::play()
{
    if(m_status == Fw::TS_Pause)
    {
        m_status = Fw::TS_Start;
        startTaskTimer();
    }
}

void FwSchedulerTask::pause()
{
    if(m_status == Fw::TS_Start)
    {
        m_status = Fw::TS_Pause;
        killTaskTimer();
    }
}

bool FwSchedulerTask::event(QEvent * e)
{
    if(e->type() == FwSchedulerTaskEvent::typeID())
    {
        FwSchedulerTaskEvent* taskEvent = static_cast<FwSchedulerTaskEvent*>(e);
        switch(taskEvent->status())
        {
        case Fw::TS_Start:
            start();
            taskEvent->accept();
            return true;

        case Fw::TS_Stop:
            stop();
            taskEvent->accept();
            return true;
        }
    }
    return BaseClass::event(e);
}

void FwSchedulerTask::setInterval(int interval)
{
    if(m_interval != interval)
    {
        m_interval = interval;
        if(m_timerId)
        {
            killTaskTimer();
            startTaskTimer();
        }
    }
}

void FwSchedulerTask::timerEvent(QTimerEvent* event)
{
    if(event->timerId() == m_timerId && m_status == Fw::TS_Start)
    {
        run();
    }
}

void FwSchedulerTask::startTaskTimer()
{
    if(!m_timerId && m_interval)
    {
        m_timerId = startTimer(m_interval);
    }
}

void FwSchedulerTask::killTaskTimer()
{
    if(m_timerId)
    {
        killTimer(m_timerId);
        m_timerId = 0;
    }
}

void FwSchedulerTask::setRunOnStart(bool enable)
{
    m_runOnStart = enable;
}
////////////////////////////////////////////////////////////////////////////////

FwSystemSchedulerTask::FwSystemSchedulerTask(QObject *parent) :
    BaseClass(parent)
{
}

void FwSystemSchedulerTask::setCommand(const QString& command)
{
    if(m_command != command)
    {
        m_command = command;
    }
}

void FwSystemSchedulerTask::run()
{
    if(!m_command.isEmpty())
    {
        qDebug() << "Execute task: " << m_command;
        qDebug() << "Result:" << QProcess::execute(m_command.toUtf8());
    }
}

////////////////////////////////////////////////////////////////////////////////

FwNetworkSchedulerTask::FwNetworkSchedulerTask(QObject* parent) :
    BaseClass(parent),
    networkManager(0),
    m_masterReply(0)
{
}

FwNetworkSchedulerTask::~FwNetworkSchedulerTask()
{
    clearReply();
}

void FwNetworkSchedulerTask::run()
{
    if(m_url.isValid() && networkManager)
    {
        pause();
        m_masterReply = networkManager->get(this, m_url);
    }
}

void FwNetworkSchedulerTask::setUrl(const QUrl& url)
{
    if(m_url != url)
    {
        m_url = url;
    }
}

void FwNetworkSchedulerTask::replyFinished(QNetworkReply* reply)
{
    bool masterReplay = (m_masterReply == reply);
    bool slaveReply = (masterReplay ? false : m_slaves.contains(reply));
    if(masterReplay || slaveReply)
    {
        bool canPlay = true;

        if(reply->error() == QNetworkReply::NoError)
        {
            if(masterReplay)
            {
                canPlay = replyMasterProcessed(reply);
            }
            else if(slaveReply)
            {
                replySlaveProcessed(reply);
                m_slaves.removeAll(reply);
            }
            reply->close();
        }
        else
        {
            clearReply();
        }

        if(canPlay && m_slaves.isEmpty())
        {
            play();
        }
    }
}

QPointer<QNetworkReply> FwNetworkSchedulerTask::get(const QUrl& url)
{
    if(networkManager && url.isValid())
    {
        pause();
        QNetworkReply* reply = networkManager->get(this, url);
        m_slaves.append(reply);
        return QPointer<QNetworkReply>(reply);
    }
    return QPointer<QNetworkReply>();
}

void FwNetworkSchedulerTask::replySlaveProcessed(QNetworkReply* reply)
{
    Q_UNUSED(reply);
}

void FwNetworkSchedulerTask::clearReply()
{
    if(m_masterReply)
    {
        m_masterReply->close();
        m_masterReply->abort();
        m_masterReply->deleteLater();
        m_masterReply = 0;
    }

    foreach(QPointer<QNetworkReply> reply, m_slaves)
    {
        if(reply)
        {
            reply->abort();
            reply->deleteLater();
        }
    }
    m_slaves.clear();
}

////////////////////////////////////////////////////////////////////////////////

FwSchedulerTaskEvent::FwSchedulerTaskEvent() :
    BaseClass(static_cast<QEvent::Type>(typeID())),
    m_status(Fw::TS_Unknow)
{
}

int FwSchedulerTaskEvent::typeID()
{
    static int _typeID = QEvent::registerEventType();
    return _typeID;
}

void FwSchedulerTaskEvent::setStatus(int status)
{
    m_status = status;
}
