#include <QtCore/qcoreapplication.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qfile.h>
#include <QtCore/qdebug.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qdatetime.h>

#include <iostream>


#include "fwcore/fwml.h"

#include "fwutils/fwnetworkconfig.h"

using namespace std;

static QString execCommand( const QString& command, const QStringList& params, QString path)
{
    if(command.isEmpty())
    {
        return QString();
    }
    else if(command.toLower() == "network")
    {

        QTextStream qout(stdout, QFile::WriteOnly);
        FwNetworkConfig *networkConfig = new FwNetworkConfig();
        QFile fwmlFile(path);
        if(fwmlFile.exists())
        {
            FwMLObject rootObject;
            if(rootObject.parse(&fwmlFile))
            {
                networkConfig->apply(&rootObject);
            }
        }
        qout << networkConfig->activeInterfaceName() << ":" << endl;
        qout << "ip : " << networkConfig->ip().toString() << endl;
        qout << "net mask : " << networkConfig->netmask().toString() << endl;
        qout << "1st DNS : " << networkConfig->firstDNS().toString() << endl;
        qout << "2nd DNS : " << networkConfig->secondDNS().toString() << endl;
        qout << "gateway : " << networkConfig->gateway().toString() << endl;

    }
    else
    {
        return QString("Unknow command: %1").arg(command);
    }
}

static void interactive(QString path)
{
    QTextStream qin(stdin, QFile::ReadOnly);
    const char prompt[] = "systeminfo> ";
    forever {
        QString line;

        printf("%s", prompt);
        fflush(stdout);

        line = qin.readLine();
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
                    QString error = execCommand(command, params, path);
                    if(!error.isEmpty())
                    {
                        fprintf(stdout, "Error: %s\n", qPrintable(error));
                    }
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    //new string adds
    QCoreApplication a(argc, argv);

    QStringList args = a.arguments();
    if(args.size() >= 2)
    {
        args.takeFirst();
        QString error = execCommand(args.takeFirst(), args, a.applicationDirPath() + QDir::separator() + "network.fwml");
        if(!error.isEmpty())
        {
            qFatal(qPrintable(error));
            return -1;
        }
    }
    else
    {
        interactive(a.applicationDirPath() + QDir::separator() + "network.fwml");
    }

    return 0;
}
