#include <QtCore/qcoreapplication.h>
#include <QtCore/qprocess.h>
#include <QtCore/qdebug.h>

#include "fw/scheduler.hpp"

#include "fwcore/fwml.h"
#include "fwutils/fwconfig.h"

Fw::Scheduler::Scheduler(QObject* parent, const QByteArray& name) :
    BaseClass(name),
    QThread(parent),
    networkManager(0)
{
    networkManager = new Scheduler::NetworkManager();
    networkManager->setParent(0);
    networkManager->moveToThread(this);
}

Fw::Scheduler::~Scheduler()
{
}

int Fw::Scheduler::addTask(Fw::Scheduler::Task* task)
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

        Fw::Scheduler::NetworkTask* networkTask = qobject_cast<Fw::Scheduler::NetworkTask*>(task);
        if(networkTask)
        {
            networkTask->networkManager = networkManager;
        }
    }

    return id;
}

bool Fw::Scheduler::startTask(Fw::Scheduler::Task* task)
{
    if(task)
    {
        Fw::Scheduler::TaskEvent* event = new Fw::Scheduler::TaskEvent();
        event->setStatus(Fw::TS_Start);
        QCoreApplication::postEvent(task, event);
        return true;
    }
    return false;
}

bool Fw::Scheduler::stopTask(Fw::Scheduler::Task* task)
{
    if(task)
    {
        Fw::Scheduler::TaskEvent* event = new Fw::Scheduler::TaskEvent();
        event->setStatus(Fw::TS_Stop);
        QCoreApplication::postEvent(task, event);
        return true;
    }
    return false;
}

void Fw::Scheduler::startAllTasks()
{
    foreach(Fw::Scheduler::Task* task, m_tasks.values())
    {
        startTask(task);
    }
}

void Fw::Scheduler::stopAllTasks()
{
    foreach(Fw::Scheduler::Task* task, m_tasks.values())
    {
        stopTask(task);
    }
}

bool Fw::Scheduler::postEvent(int taskId, QEvent* event)
{
    Fw::Scheduler::Task* task = m_tasks.value(taskId, 0);
    if(task)
    {
        QCoreApplication::postEvent(task, event);
        return true;
    }
    return false;
}

QList<Fw::Scheduler::Task*> Fw::Scheduler::loadTasks(const QString& filename, QString* error)
{
    QFileInfo fInfo(filename);
    if(!fInfo.exists())
    {
        (*error) = QString("File %1 not found").arg(filename);
        return QList<Fw::Scheduler::Task*>();
    }

    QFile tasksFile(filename);
    if(!tasksFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        (*error) = tasksFile.errorString();
        return QList<Fw::Scheduler::Task*>();
    }

    QList<Fw::Scheduler::Task*> tasks;

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
                        Fw::Scheduler::SystemTask* task = new Fw::Scheduler::SystemTask(0, command.toUtf8());
                        task->setCommand(command);
                        task->setRunOnStart(true);
                        task->setInterval(time * 1000);
                        tasks.append(task);
                        continue;
                    }
                }
            }

            (*error) = QString("Invalid file structure, line %1").arg(lineIndex);
            foreach(Fw::Scheduler::Task* task, tasks)
            {
                task->deleteLater();
            }
            tasks.clear();
            return QList<Fw::Scheduler::Task*>();
        }
    }

    return tasks;
}

void Fw::Scheduler::run()
{
    QThread::run();
    release();
}

void Fw::Scheduler::addShellTasks(FwMLObject* object)
{
    FwMLArray* taskArray = object->attribute("ShellCommands")->cast<FwMLArray>();
    if(taskArray)
    {
        QVector<FwMLNode*> taskNodes = taskArray->toQVector();

        foreach(FwMLNode* taskNode, taskNodes)
        {
            FwMLObject* taskObject = taskNode->cast<FwMLObject>();
            if(taskObject)
            {
                SystemTask* task = new SystemTask();
                if(task->loadData(taskObject))
                {
                    addTask(task);
                }
                else
                {
                    delete task;
                    task = 0;
                }
            }
        }
    }
}

void Fw::Scheduler::release()
{
    foreach(Fw::Scheduler::Task* task, m_tasks.values())
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

bool Fw::Scheduler::loadConfig() throw(Fw::Exception&)
{
    FwConfig::loadConfig("scheduler", this);
    return true;
}

bool Fw::Scheduler::loadData(FwMLObject *object)
{
    addShellTasks(object);

    foreach(Fw::Scheduler::Task* task, m_tasks)
    {
        if(!task->name().isEmpty())
        {
            FwMLObject* node = object->attribute(task->name())->cast<FwMLObject>();
            if(node)
            {
                task->loadData(node);
            }
        }
    }
    return true;
}

////////////////////////////////////////////////////////////////////

Fw::Scheduler::NetworkManager::NetworkManager(QObject* parent) :
    BaseClass(parent)
{
    connect(this, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
}

QNetworkReply* Fw::Scheduler::NetworkManager::get(Fw::Scheduler::NetworkTask* task, const QUrl& url)
{
    QNetworkReply* reply = BaseClass::get(QNetworkRequest(url));
    if(reply)
    {
        replyCache.insert(qHash(reply), task);
        connect(reply, SIGNAL(destroyed(QObject*)), this, SLOT(replyDestroy(QObject*)));
    }
    return reply;
}

void Fw::Scheduler::NetworkManager::replyFinished(QNetworkReply* reply)
{
    int hash = qHash(reply);
    if(replyCache.contains(hash))
    {
        QPointer<Fw::Scheduler::NetworkTask> task = replyCache.value(hash, 0);
        if(task)
        {
            task->replyFinished(reply);
        }
        reply->deleteLater();
        reply = 0;
        replyCache.remove(hash);
    }
}

void Fw::Scheduler::NetworkManager::replyDestroy(QObject* object)
{
    int hash = qHash(object);
    if(replyCache.contains(hash))
    {
        replyCache.remove(hash);
    }
}

////////////////////////////////////////////////////////////////////

Fw::Scheduler::Task::Task(QObject* parent, const QByteArray& name) :
    QObject(parent),
    BaseClass(name),
    scheduler(0),
    m_timerId(0),
    m_status(Fw::TS_Stop),
    m_interval(0),
    m_runOnStart(true)
{
}

Fw::Scheduler::Task::~Task()
{
    stop();
    Q_ASSERT(scheduler);
}

void Fw::Scheduler::Task::stop()
{
    if(m_status != Fw::TS_Stop)
    {
        m_status = Fw::TS_Stop;
        killTaskTimer();
    }
}

void Fw::Scheduler::Task::start()
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

void Fw::Scheduler::Task::play()
{
    if(m_status == Fw::TS_Pause)
    {
        m_status = Fw::TS_Start;
        startTaskTimer();
    }
}

void Fw::Scheduler::Task::pause()
{
    if(m_status == Fw::TS_Start)
    {
        m_status = Fw::TS_Pause;
        killTaskTimer();
    }
}

bool Fw::Scheduler::Task::event(QEvent * e)
{
    if(e->type() == Fw::Scheduler::TaskEvent::typeID())
    {
        Fw::Scheduler::TaskEvent* taskEvent = static_cast<Fw::Scheduler::TaskEvent*>(e);
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
    return QObject::event(e);
}

void Fw::Scheduler::Task::setInterval(int msecs)
{
    if(m_interval != msecs)
    {
        m_interval = msecs;
        if(m_timerId)
        {
            killTaskTimer();
            startTaskTimer();
        }
    }
}

void Fw::Scheduler::Task::timerEvent(QTimerEvent* event)
{
    if(event->timerId() == m_timerId && m_status == Fw::TS_Start)
    {
        run();
    }
}

void Fw::Scheduler::Task::startTaskTimer()
{
    if(!m_timerId && m_interval)
    {
        m_timerId = startTimer(m_interval);
    }
}

void Fw::Scheduler::Task::killTaskTimer()
{
    if(m_timerId)
    {
        killTimer(m_timerId);
        m_timerId = 0;
    }
}

void Fw::Scheduler::Task::setRunOnStart(bool enable)
{
    m_runOnStart = enable;
}

bool Fw::Scheduler::Task::loadData(FwMLObject *object)
{
    FwMLUIntNumber* intervalNode = object->attribute("interval")->cast<FwMLUIntNumber>();
    if(intervalNode && intervalNode->value())
    {
        setInterval(intervalNode->value() * 1000);
    }

    FwMLBool* runOnStartNode = object->attribute("runOnStart")->cast<FwMLBool>();
    if(runOnStartNode)
    {
        setRunOnStart(runOnStartNode->value());
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////

Fw::Scheduler::SystemTask::SystemTask(QObject* parent, const QByteArray& name) :
    BaseClass(parent, name)
{
}

bool Fw::Scheduler::SystemTask::loadData(FwMLObject *object)
{
    if(!BaseClass::loadData(object))
    {
        return false;
    }

    FwMLString* commandNode = object->attribute("command")->cast<FwMLString>();
    if(commandNode)
    {
        setCommand(commandNode->value());
    }

    return true;
}

void Fw::Scheduler::SystemTask::setCommand(const QString& command)
{
    if(m_command != command)
    {
        m_command = command;
    }
}

void Fw::Scheduler::SystemTask::run()
{
    if(!m_command.isEmpty())
    {
        qDebug() << "Execute task: " << m_command;
        qDebug() << "Result:" << QProcess::execute(m_command.toUtf8());
    }
}

////////////////////////////////////////////////////////////////////////////////

Fw::Scheduler::NetworkTask::NetworkTask(QObject* parent, const QByteArray& name) :
    BaseClass(parent, name),
    networkManager(0),
    m_masterReply(0)
{
}

Fw::Scheduler::NetworkTask::~NetworkTask()
{
    clearReply();
}

void Fw::Scheduler::NetworkTask::run()
{
    if(m_url.isValid() && networkManager)
    {
        pause();
        m_masterReply = networkManager->get(this, m_url);
    }
}

void Fw::Scheduler::NetworkTask::setUrl(const QUrl& url)
{
    if(m_url != url)
    {
        m_url = url;
    }
}

void Fw::Scheduler::NetworkTask::replyFinished(QNetworkReply* reply)
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
            qDebug() << "ERROR: FwScheduler::NetworkTask: " << reply->errorString();
            clearReply();
        }

        if(canPlay && m_slaves.isEmpty())
        {
            play();
        }
    }
}

QPointer<QNetworkReply> Fw::Scheduler::NetworkTask::get(const QUrl& url)
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

void Fw::Scheduler::NetworkTask::replySlaveProcessed(QNetworkReply* reply)
{
    Q_UNUSED(reply);
}

void Fw::Scheduler::NetworkTask::clearReply()
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

bool Fw::Scheduler::NetworkTask::loadData(FwMLObject *object)
{
    if(!BaseClass::loadData(object))
    {
        return false;
    }

    FwMLString* urlNode = object->attribute("url")->cast<FwMLString>();
    if(urlNode)
    {
        if(!urlNode->value().isEmpty())
        {
            setUrl(QString::fromUtf8(urlNode->value()));
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////

Fw::Scheduler::TaskEvent::TaskEvent() :
    BaseClass(static_cast<QEvent::Type>(typeID())),
    m_status(Fw::TS_Unknow)
{
}

int Fw::Scheduler::TaskEvent::typeID()
{
    static int _typeID = QEvent::registerEventType();
    return _typeID;
}

void Fw::Scheduler::TaskEvent::setStatus(int status)
{
    m_status = status;
}
