#include <QtCore/qtextstream.h>
#include <QtCore/qstringlist.h>

#include "fwconsole.h"

FwConsole::FwConsole(QObject* parent) :
    BaseClass(parent),
    m_receiver(0)
{
}

void FwConsole::writeLine(const QString& line)
{
    QTextStream qout(stdout, QIODevice::WriteOnly);
    qout << line << endl;
    qout.flush();
}

bool FwConsole::execCommand(const QString& command, const QStringList& params)
{
    if(m_commands.contains(command))
    {
        CommandFunc commandFunction = m_commands.value((command));
        return commandFunction(m_receiver, params, &errorMessage);
    }
    errorMessage = QString("Unknow command %1").arg(command);
    return false;
}

void FwConsole::interactiveMode(const QByteArray& promt)
{
    QTextStream qin(stdin, QIODevice::ReadOnly);
    QTextStream qout(stdout, QIODevice::WriteOnly);

    forever
    {
        qout << promt << "> ";
        qout.flush();

        QString line = qin.readLine();
        if (line.isNull())
        {
            break;
        }

        if(!line.trimmed().isEmpty())
        {
            QStringList params = line.split(' ');
            if(!params.isEmpty())
            {
                QString command = params.takeFirst();
                if(command == "exit" || command == "quit")
                {
                    return;
                }
                else
                {
                    if(!execCommand(command, params))
                    {
                        qout << "ERROR :"<< errorMessage << endl;
                        qout.flush();
                    }

                }
            }
        }
    }
}
