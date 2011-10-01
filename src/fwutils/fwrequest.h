#ifndef FIREWORKS_REQUEST_H
#define FIREWORKS_REQUEST_H

#include <QtCore/qobject.h>
#include <QtCore/qpointer.h>
#include <QtCore/quuid.h>
#include <QtCore/qcoreevent.h>

#include <QtCore/qvector.h>

#include "fireworks.h"

class FIREWORKSSHARED_EXPORT FwRequestAnswer
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

    FwRequestAnswer();
    explicit FwRequestAnswer(int result, const QByteArray &name);

    inline int result() const;
    inline const QByteArray& name() const;

    inline int key() const;
    inline void setKey(Qt::Key key);

    inline const QString& caption () const;
    inline void setCaption (const QString& caption);

    inline QByteArray icon() const;
    inline void setIcon(const QByteArray& name);

private:
    int m_key;
    int m_modalResult;
    QByteArray m_name;

    QString m_caption;
    QByteArray m_icon;
};

int FwRequestAnswer::key() const
{
    return m_key;
}

void FwRequestAnswer::setKey(Qt::Key key)
{
    m_key = key;
}

int FwRequestAnswer::result() const
{
    return m_modalResult;
}

const QString& FwRequestAnswer::caption() const
{
    return m_caption;
}

void FwRequestAnswer::setCaption(const QString &caption)
{
    m_caption = caption;
}

const QByteArray& FwRequestAnswer::name() const
{
    return m_name;
}


QByteArray FwRequestAnswer::icon() const
{
    return m_icon;
}

void FwRequestAnswer::setIcon(const QByteArray& name)
{
    m_icon = name;
}

/////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT FwRequest
{
public:
    FwRequest(QObject *sender = 0, const QString& text = QString());

    inline bool isNull() const;

    inline QPointer<QObject> sender() const;
    inline QUuid id() const;

    inline QString text() const;
    inline void setText(const QString& text);

    inline QString title() const;
    inline void setTitle(const QString& title);

    QUuid postRequest(QObject* receiver);
    void postAnswer(int result);

    inline FwRequestAnswer& addAnswer(int result, const QByteArray &name);

    int result(int key);

    inline QVector<FwRequestAnswer> answers() const;

private:
    QUuid m_uid;
    QPointer<QObject> m_sender;
    QVector<FwRequestAnswer> m_answers;
    QString m_text;
    QString m_title;
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

QString FwRequest::title() const
{
    return m_title;
}

void FwRequest::setTitle(const QString& title)
{
    m_title = title;
}

bool FwRequest::isNull() const
{
    return !m_sender;
}

QVector<FwRequestAnswer> FwRequest::answers() const
{
    return m_answers;
}

QUuid FwRequest::id() const
{
    return m_uid;
}

FwRequestAnswer& FwRequest::addAnswer(int result, const QByteArray &name)
{
    m_answers.append(FwRequestAnswer(result, name));
    return m_answers.last();
}

/////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT FwPostRequestEvent : public QEvent
{
    typedef QEvent BaseClass;

public:
    FwPostRequestEvent(const FwRequest& request);

    static int typeID();

    inline const FwRequest& request() const;

private:
    FwRequest m_request;
};

const FwRequest& FwPostRequestEvent::request() const
{
    return m_request;
}

////////////////////////////////////////////////////////////////////////

class FIREWORKSSHARED_EXPORT FwResult: public QEvent
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
