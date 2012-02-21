
#include <QtCore/qdebug.h>
#include <QtCore/QDateTime>
#include <QFile>

#include "cjson/cJSON.h"

#include "globals.hpp"
#include "jsonrpc.hpp"

const QByteArray GetNetworkMACAddres()
{
    return "00:11:95:fc:71:3b";
}

QByteArray TimestampToStringDate(uint timestamp)
{
    QDateTime dateTime;
    dateTime.setUtcOffset(0);
    dateTime = dateTime.fromTime_t(timestamp);

    return dateTime.toString(Qt::ISODate).toUtf8();
}

inline void MakeJsonRequestHeader(cJSON* object, qint32 requestID, const QByteArray& method)
{
    cJSON_AddNumberToObject(object, "jsonrpc", 2.0);
    cJSON_AddNumberToObject(object, "id", requestID);
    cJSON_AddStringToObject(object, "method", method.constData());
}

QByteArray GetJsonRequestPlaylist(qint32 requestID, const QByteArray& macAddres)
{
    cJSON* coreObject = cJSON_CreateObject();

    MakeJsonRequestHeader(coreObject, requestID, "get_playlists");

    cJSON* objParams = cJSON_CreateObject();
    cJSON_AddItemToObject(coreObject, "params", objParams);

    cJSON_AddStringToObject(objParams, "macaddr", macAddres);

    char* text = cJSON_PrintUnformatted(coreObject);
    const QByteArray result(text);
    free(text);
    text = 0;

    cJSON_Delete(coreObject);
    coreObject = 0;

    return result;
}

QByteArray GetJsonRequestEPG(qint32 requestID, qint32 channelID, qint32 limit, qint32 timestampStart, qint32 timestampStop)
{
    cJSON* coreObject = cJSON_CreateObject();

    MakeJsonRequestHeader(coreObject, requestID, "get_epg");

    cJSON* objParams = cJSON_CreateObject();
    cJSON_AddItemToObject(coreObject, "params", objParams);

    cJSON_AddNumberToObject(objParams, "channel_id", channelID);
    cJSON_AddNumberToObject(objParams, "limit", limit);

    cJSON_AddStringToObject(objParams, "start", TimestampToStringDate(timestampStart));
    cJSON_AddStringToObject(objParams, "stop", TimestampToStringDate(timestampStop));

    char* text = cJSON_PrintUnformatted(coreObject);
    const QByteArray result(text);
    free(text);
    text = 0;

    cJSON_Delete(coreObject);
    coreObject = 0;

    return result;
}

QString ParseJSONString(const QString& string)
{
    QString out;
    out.reserve(string.size());

    QString::ConstIterator iter = string.begin();
    QString::ConstIterator end = string.end();

    while(iter != end)
    {
        if(*iter != QChar('\\'))
        {
            out.append(*iter);
            ++iter;
            continue;
        }

        if((++iter) == end)
        {
            throw QString();
        }

        switch(iter->unicode())
        {
        case 'b':
            out.append(QChar('\b'));
            break;

        case 'f':
            out.append(QChar('\f'));
            break;

        case 'n':
            out.append(QChar('\n'));
            break;

        case 'r':
            out.append(QChar('\r'));
            break;

        case 't':
            out.append(QChar('\t'));
            break;

        case '\\':
            out.append(QChar('\\'));
            break;

        case 'u':
            if((end - iter) >= 5)
            {
                bool ok = false;
                short uch = QString(iter+1, 4).toShort(&ok, 16);
                if(!ok)
                {
                    throw QString();
                }

                out.append(QChar(uch));

                iter += 5;
                continue;
            }
            break;

        default:
            throw QString();
        }

        ++iter;
    }

    return out;
}

JsonRpc::JsonRpc(QCoreApplication* parent) :
BaseClass(parent),
m_app(parent),
m_networkManager(0)
{
    m_networkManager = new QNetworkAccessManager(this);

    BaseClass::connect(m_networkManager, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(replyFinished(QNetworkReply*)));

    makeRequestPlaylist(0);
}

JsonRpc::~JsonRpc()
{
    m_app = 0;

//    delete m_networkManager;
    m_networkManager = 0;
}

void JsonRpc::makeRequestPlaylist(qint32 requestID)
{
    const QByteArray jsonRequest = GetJsonRequestPlaylist(requestID, GetNetworkMACAddres());

    qDebug() << "Playlist request : ";
    qDebug() << jsonRequest;
    qDebug() << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";

    //QNetworkReply* reply = m_networkManager->post(QNetworkRequest(QUrl(APIServerURL)), jsonRequest);
    //m_replysPlaylist.insert(reply);
}

void JsonRpc::makeRequestEPG(qint32 channelID, qint32 limit, const QDateTime& begin, const QDateTime& end)
{
    const QByteArray jsonRequest = GetJsonRequestEPG(channelID, channelID, limit, begin.toTime_t(), end.toTime_t());

    qDebug() << "EPG request : ";
    qDebug() << jsonRequest;
    qDebug() << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";

    QNetworkReply* reply = m_networkManager->post(QNetworkRequest(QUrl(APIServerURL)), jsonRequest);
    m_replysEPG.insert(reply);
}

void JsonRpc::parseJsonReply(QNetworkReply* reply)
{
    const QByteArray replyBuffer = reply->readAll();

    qDebug() << "parseJsonReply:";
    qDebug() << replyBuffer;
    qDebug() << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
    qDebug() << "error: ";
    qDebug() << reply->error();

    if(cJSON* object = cJSON_Parse(replyBuffer))
    {
        if(m_replysPlaylist.remove(reply))
        {
            parseJsonReplyPlaylist(object);
        }
        else if(m_replysEPG.remove(reply))
        {
            parseJsonReplyEPG(object);

            QFile file("./programs/EPG.txt");
            if(file.open(QIODevice::WriteOnly|QIODevice::Truncate))
            {
                file.write(replyBuffer);
                file.close();
            }
        }

        reply->deleteLater();
        reply = 0;

        cJSON_Delete(object);
        object = 0;
    }
}

void JsonRpc::parseJsonReplyPlaylist(cJSON* object)
{
    m_chCategoryes.clear();
    m_channels.clear();
    m_epg.clear();

    cJSON* objectValue = object->child;
    while(objectValue)
    {
        if(objectValue->type == cJSON_Object && QString("result") == objectValue->string)
        {
            cJSON* objectResult = objectValue->child;
            while(objectResult)
            {
                if(objectResult->type == cJSON_Array)
                {
                     if(QString("playlists") == objectResult->string)
                     {
                         parsePlaylistArray(objectResult);
                     }
                     else if(QString("medialist") == objectResult->string)
                     {
                         parseMedialistArray(objectResult);
                     }
                }
                objectResult = objectResult->next;
            }
        }
        objectValue = objectValue->next;
    }

    foreach(const CASChannel& channel, m_channels)
    {
        makeRequestEPG(channel.id(), 99, channel.epgStart(), channel.epgStop());
    }

    QFile file("./programs/Playlist.txt");
    if(file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        foreach(const CASChannelCategory& chCategory, m_chCategoryes)
        {
            file << chCategory;
        }

        foreach(const CASChannel& channel, m_channels)
        {
            file << channel;
        }

//        file.write("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
//        file.write(replyBuffer);
        file.close();
    }
}

void JsonRpc::parseJsonReplyEPG(cJSON* object)
{
    qint32 channelID = 0;
    QDateTime now;

    cJSON* objectValue = object->child;
    while(objectValue)
    {
        if(objectValue->type == cJSON_Number && QString("id") == objectValue->string)
        {
            channelID = objectValue->valueint;
        }
        else if(objectValue->type == cJSON_Object && QString("result") == objectValue->string)
        {
            cJSON* objectResult = objectValue->child;
            while(objectResult)
            {
                 if(QString("now") == objectResult->string && objectResult->type == cJSON_String)
                 {
                     now = QDateTime::fromString(objectResult->valuestring, Qt::ISODate);
                 }
                 else if(QString("programs") == objectResult->string && objectResult->type == cJSON_Array)
                 {
                     parseProgramsArray(objectResult, channelID);
                 }

                objectResult = objectResult->next;
            }
        }
        objectValue = objectValue->next;
    }
}

void JsonRpc::parsePlaylistArray(cJSON* object)
{
    cJSON* objectCategory = object->child;
    while(objectCategory)
    {
        CASChannelCategory chCategory(objectCategory);
        m_chCategoryes.insert(chCategory.id(), chCategory);

        objectCategory = objectCategory->next;
    }
}

void JsonRpc::parseMedialistArray(cJSON* object)
{
    cJSON* objectChannel = object->child;
    while(objectChannel)
    {
        CASChannel channel(objectChannel);
        m_channels.insert(channel.id(), channel);

        objectChannel = objectChannel->next;
    }
}

void JsonRpc::parseProgramsArray(cJSON* object, qint32 channelID)
{
    Programs& programs = *m_epg.insert(channelID, Programs());

    cJSON* objectProgram = object->child;
    while(objectProgram)
    {
        CASChannelProgram program(objectProgram);
        programs.push_back(program);

        objectProgram = objectProgram->next;
    }

    const CASChannel& channel = m_channels.value(channelID, CASChannel::DefaultObject());
    QFile file(QString("./programs/program_%1_%2.txt").arg(channelID).arg(channel.name()));
    if(file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        foreach(const CASChannelProgram& program, programs)
        {
            file << program;
        }

        file.close();
    }
}

void JsonRpc::replyFinished(QNetworkReply* reply)
{
    this->parseJsonReply(reply);

    if(m_channels.size() == m_epg.size() && m_app)
    {
        m_app->exit();
    }
}
