#ifndef FIREWORKS_SCHEDULER_HPP
#define FIREWORKS_SCHEDULER_HPP

#include <QtCore/qthread.h>
#include <QtCore/qhash.h>
#include <QtCore/qpointer.h>
#include <QtCore/qurl.h>
#include <QtCore/qmap.h>
#include <QtCore/qcoreevent.h>
#include <QtCore/QEvent>

#include <QtNetwork/qnetworkaccessmanager.h>
#include <QtNetwork/qnetworkreply.h>

#include "fireworks.h"

#include "fwcore/fwcppobject.h"

class QNetworkReply;
class QNetworkAccessManager;

namespace Fw
{
    class Scheduler;
    //class Scheduler::Task;

}

class FIREWORKSSHARED_EXPORT Fw::Scheduler : public QThread, public FwCPPObject
{
    Q_OBJECT
    typedef FwCPPObject BaseClass;

public:

    class Task;
    class SystemTask;
    class NetworkTask;
    class NetworkManager;
    class TaskEvent;

    friend class Task;

    explicit Scheduler(QObject* parent = 0, const QByteArray& name = QByteArray());
    virtual ~Scheduler();

    int addTask(Task* task);

    inline bool startTask(int taskId);
    bool startTask(Task* task);

    inline bool stopTask(int taskId);
    bool stopTask(Task* task);

    void startAllTasks();
    void stopAllTasks();

    bool postEvent(int taskId, QEvent* event);

    static QList<Task*> loadTasks(const QString& filename, QString* error);

    inline QString lastError() const;

    void release();

    bool loadConfig() throw(Fw::Exception&);
    bool loadData(FwMLObject* object);

protected:
    void run();

    void addShellTasks(FwMLObject* object);

private:
    QPointer<NetworkManager> networkManager;

    QHash<int, Task*> m_tasks;
    QString errorString;
};

////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT Fw::Scheduler::NetworkManager : public QNetworkAccessManager
{
    Q_OBJECT
    typedef QNetworkAccessManager BaseClass;

public:
    NetworkManager(QObject* parent = 0);

    QNetworkReply* get(Scheduler::NetworkTask* task, const QUrl& url);

private:
    QHash<int, QPointer<Scheduler::NetworkTask> >  replyCache;

private slots:
    void replyFinished(QNetworkReply* reply);
    void replyDestroy(QObject* object);
};

////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT Fw::Scheduler::Task : public QObject, public FwCPPObject
{
    Q_OBJECT
    typedef FwCPPObject BaseClass;

public:
    friend class Scheduler;

    explicit Task(QObject* parent = 0, const QByteArray& name = QByteArray());
    virtual ~Task();

    bool event(QEvent * e);

    inline int interval() const;
    void setInterval(int msecs);

    inline bool isRunningOnStart() const;
    void setRunOnStart(bool enable);

    virtual bool loadData(FwMLObject *object);

protected:
    void stop();
    void start();

    void play();
    void pause();

    void startTaskTimer();
    void killTaskTimer();

    void timerEvent(QTimerEvent* event);

    virtual void run() = 0;

private:
    int m_timerId;
    int m_interval;
    int m_status;
    bool m_runOnStart;
    QPointer<Fw::Scheduler> scheduler;
};

////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT Fw::Scheduler::SystemTask : public Fw::Scheduler::Task
{
    Q_OBJECT
    typedef Scheduler::Task BaseClass;

public:
    SystemTask(QObject* parent = 0, const QByteArray& name = QByteArray());

    virtual bool loadData(FwMLObject *object);

    inline QString command() const;
    void setCommand(const QString& command);

protected:
    virtual void run();

private:
    QString m_command;
};

////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT Fw::Scheduler::NetworkTask : public Fw::Scheduler::Task
{
    Q_OBJECT
    typedef Fw::Scheduler::Task BaseClass;

public:
    friend class Scheduler;
    friend class SchedulerNetworkManager;

    NetworkTask(QObject* parent = 0, const QByteArray& name = QByteArray());
    virtual ~NetworkTask();

    inline QUrl url() const;
    void setUrl(const QUrl& url);

    void clearReply();

    bool loadData(FwMLObject *object);

protected:
    void run();

    QPointer<Scheduler::NetworkManager> networkManager;

    virtual bool replyMasterProcessed(QNetworkReply* reply) = 0;
    virtual void replySlaveProcessed(QNetworkReply* reply);

    QPointer<QNetworkReply> get(const QUrl& url);

private:
    void replyFinished(QNetworkReply*);

    QUrl m_url;
    QPointer<QNetworkReply> m_masterReply;
    QList< QPointer<QNetworkReply> > m_slaves;
};

////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT Fw::Scheduler::TaskEvent: public QEvent
{
    typedef QEvent BaseClass;
public:
    TaskEvent();

    static int typeID();

    inline int status() const;
    void setStatus(int status);

private:
    int m_status;
};

#include "fw/scheduler_inl.hpp"

#endif // FIREWORKS_SCHEDULER_HPP
