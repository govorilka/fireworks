
#ifndef JSONRPC_HPP
#define JSONRPC_HPP

#include <QtCore/QCoreApplication>
#include <QtNetwork/qnetworkaccessmanager.h>
#include <QtNetwork/qnetworkreply.h>
#include <QtCore/QMap>

#include "caschannel.hpp"
#include "caschannelcategory.hpp"

QString ParseJSONString(const QString& string);

class JsonRpc : public QObject
{
    Q_OBJECT
    typedef QObject BaseClass;

public:
    explicit JsonRpc(QCoreApplication* parent = 0);
    virtual ~JsonRpc();

protected :
    void makeRequestPlaylist(qint32 requestID);
    void makeRequestEPG(qint32 channelID, qint32 limit, const QDateTime& begin, const QDateTime& end);

    void parseJsonReply(QNetworkReply* reply);

    void parseJsonReplyPlaylist(cJSON* object);
    void parseJsonReplyEPG(cJSON* object);

    void parsePlaylistArray(cJSON* object);
    void parseMedialistArray(cJSON* object);

    void parseProgramsArray(cJSON* object, qint32 channelID);

protected slots:
    void replyFinished(QNetworkReply* reply);

private:
    QCoreApplication* m_app;
    QNetworkAccessManager* m_networkManager;

    typedef QSet<QNetworkReply*> ReplyMap;
    typedef QMap<qint32, CASChannel> Channels;
    typedef QMap<qint32, CASChannelCategory> ChannelCategoryes;
    typedef QList<CASChannelProgram> Programs;
    typedef QMap<qint32, Programs> EPG;

    ReplyMap m_replysPlaylist;
    ReplyMap m_replysEPG;

    Channels m_channels;
    ChannelCategoryes m_chCategoryes;
    EPG m_epg;
};

#endif // JSONRPC_HPP
