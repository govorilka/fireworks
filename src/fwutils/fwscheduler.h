#ifndef FIREWORKS_SCHEDULER_H
#define FIREWORKS_SCHEDULER_H

#include <QtCore/qthread.h>
#include <QtCore/qhash.h>
#include <QtCore/qpointer.h>
#include <QtCore/qurl.h>
#include <QtCore/qmap.h>
#include <QtCore/qcoreevent.h>

#include <QtNetwork/qnetworkaccessmanager.h>
#include <QtNetwork/qnetworkreply.h>

#include "fireworks.h"

#include "fwcore/fwcppobject.h"

class QNetworkReply;
class QNetworkAccessManager;

class FwSchedulerTask;
class FwNetworkSchedulerTask;
class FwSchedulerNetworkManager;

class FIREWORKSSHARED_EXPORT FwScheduler : public QThread, public FwCPPObject
{
    Q_OBJECT
    typedef FwCPPObject BaseClass;

public:
    friend class FwSchedulerTask;

    explicit FwScheduler(const QByteArray& name,QObject *parent = 0);
    virtual ~FwScheduler();

    int addTask(FwSchedulerTask* task);

    inline bool startTask(int taskId);
    inline bool stopTask(int taskId);

    bool startTask(FwSchedulerTask* task);
    bool stopTask(FwSchedulerTask* task);

    void startAllTasks();
    void stopAllTasks();

    bool postEvent(int taskId, QEvent* event);

    static QList<FwSchedulerTask*> loadTasks(const QString& filename, QString* error);

    inline QString lastError() const;

    void release();

    bool loadConfig();

    void apply(FwMLObject *object);

protected:
    void run();

private:
    QPointer<FwSchedulerNetworkManager> networkManager;

    QHash<int, FwSchedulerTask*> m_tasks;
    QString errorString;
};

////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT FwSchedulerNetworkManager : public QNetworkAccessManager
{
    Q_OBJECT
    typedef QNetworkAccessManager BaseClass;

public:
    FwSchedulerNetworkManager(QObject* parent = 0);

    QNetworkReply* get(FwNetworkSchedulerTask* task, const QUrl& url);

private:
    QHash<int, QPointer<FwNetworkSchedulerTask> >  replyCache;

private slots:
    void replyFinished(QNetworkReply* reply);
    void replyDestroy(QObject* object);
};

////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT FwSchedulerTask : public QObject, public FwCPPObject
{
    Q_OBJECT
    typedef FwCPPObject BaseClass;

public:
    friend class FwScheduler;

    explicit FwSchedulerTask(const QByteArray& name, QObject *parent = 0);
    virtual ~FwSchedulerTask();

    bool event(QEvent * e);

    inline int interval() const;
    void setInterval(int interval);

    inline bool isRunningOnStart() const;
    void setRunOnStart(bool enable);

    void apply(FwMLObject *object);

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
    QPointer<FwScheduler> scheduler;
};

////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT FwSystemSchedulerTask : public FwSchedulerTask
{
    Q_OBJECT
    typedef FwSchedulerTask BaseClass;

public:
    FwSystemSchedulerTask(const QByteArray& name, QObject *parent = 0);

    inline QString command() const;
    void setCommand(const QString& command);

protected:
    virtual void run();

private:
    QString m_command;
};

////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT FwNetworkSchedulerTask : public FwSchedulerTask
{
    Q_OBJECT
    typedef FwSchedulerTask BaseClass;

public:
    friend class FwScheduler;
    friend class FwSchedulerNetworkManager;

    FwNetworkSchedulerTask(const QByteArray& name, QObject* parent = 0);
    virtual ~FwNetworkSchedulerTask();

    inline QUrl url() const;
    void setUrl(const QUrl& url);

    void clearReply();

    void apply(FwMLObject *object);

protected:
    void run();

    QPointer<FwSchedulerNetworkManager> networkManager;

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

class FIREWORKSSHARED_EXPORT FwSchedulerTaskEvent: public QEvent
{
    typedef QEvent BaseClass;
public:
    FwSchedulerTaskEvent();

    static int typeID();

    inline int status() const;
    void setStatus(int status);

private:
    int m_status;
};

#include "fwscheduler_inl.h"

#endif // FIREWORKS_SCHEDULER_H
