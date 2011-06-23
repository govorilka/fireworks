#ifndef FIREWORKS_REQUEST_H
#define FIREWORKS_REQUEST_H

#include <QtCore/qobject.h>
#include <QtCore/qpointer.h>
#include <QtCore/quuid.h>
#include <QtCore/qcoreevent.h>

class FwRequestAnswer
{
public:

    enum StandartResult
    {
        SR_Unknow,
        SR_No,
        SR_Yes,
        SR_Cancel = SR_No,
        SR_Ok = SR_Yes
    };

    explicit FwRequestAnswer(int result, QString caption, Qt::Key n_key);

    inline Qt::Key key() const;
    inline int result() const;

private:
    Qt::Key m_key;
    int m_modalResult;
    QString m_caption;
};

Qt::Key FwRequestAnswer::key() const
{
    return m_key;
}

int FwRequestAnswer::result() const
{
    return m_modalResult;
}

/////////////////////////////////////////////////////////////////////////

class FwRequest
{
public:
    FwRequest(QObject *sender = 0);

    inline bool isNull() const;

    inline QPointer<QObject> sender() const;
    inline QUuid id() const;

    inline QString text() const;
    inline void setText(const QString& text);

    void postRequest(QObject* receiver);
    void postAnswer(int result);

    inline void addAnswer(const FwRequestAnswer& answer);
    inline void addAnswer(int result, const QString& caption, Qt::Key key);

    int result(int key);

private:
    QUuid m_uid;
    QPointer<QObject> m_sender;
    QList<FwRequestAnswer> m_answers;
    QString m_text;
};

QPointer<QObject> FwRequest::sender() const
{
    return m_sender;
}


QString FwRequest::text() const
{
    return m_text;
}

void FwRequest::setText(const QString& text)
{
    m_text = text;
}

bool FwRequest::isNull() const
{
    return !m_sender;
}

void FwRequest::addAnswer(int result, const QString& caption, Qt::Key key)
{
    addAnswer(FwRequestAnswer(result, caption, key));
}

void FwRequest::addAnswer(const FwRequestAnswer& answer)
{
    m_answers.append(answer);
}

/////////////////////////////////////////////////////////////////////////

class FwPostRequestEvent : public QEvent
{
    typedef QEvent BaseClass;

public:
    FwPostRequestEvent(const FwRequest& request);

    static int typeID();

    inline const FwRequest& request() const;

private:
    FwRequest m_request;
};

////////////////////////////////////////////////////////////////////////

class FwResult: public QEvent
{
    typedef QEvent BaseClass;

public:
    explicit FwResult(QUuid uid, int modalResult);

    static int typeID();

    inline QUuid id() const;
    inline int result() const;

private:
    QUuid m_id;
    int m_result;
};

QUuid FwResult::id() const
{
    return m_id;
}

int FwResult::result() const
{
    return m_result;
}

#endif // FIREWORKS_REQUEST_H
