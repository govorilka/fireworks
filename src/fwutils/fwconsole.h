#ifndef FIREWORKS_CONSOLE_H
#define FIREWORKS_CONSOLE_H

#include <QtCore/qobject.h>
#include <QtCore/qhash.h>

class FwConsole : public QObject
{
    Q_OBJECT
    typedef QObject BaseClass;

public:
    static void writeLine(const QString& line);

    typedef bool(*CommandFunc)(QObject* receiver, const QStringList& params, QString* errorString);

    FwConsole(QObject* parent = 0);

    void interactiveMode(const QByteArray& promt);

    virtual bool execCommand(const QString& command, const QStringList& params);

    inline QString lastError() const;

    inline QObject* receiver() const;
    inline void setReceiver(QObject* receiver);

    bool addCommand(const QString& mode, const QString& command, CommandFunc function);
    inline bool setMode(const QString& mode);

protected:
    QString errorMessage;
    QObject* m_receiver;
    QHash<QString, CommandFunc> m_commands;
    QHash<QString, QHash<QString, CommandFunc> > m_modes;
};



QString FwConsole::lastError() const
{
    return errorMessage;
}

QObject* FwConsole::receiver() const
{
    return m_receiver;
}

void FwConsole::setReceiver(QObject *receiver)
{
   m_receiver = receiver;
}


bool FwConsole::setMode(const QString& mode)
{
    if(m_modes.contains(mode))
    {
        m_commands = m_modes.value(mode);
        return true;
    }
    return false;
}

#endif // FIREWORKS_CONSOLE_H
