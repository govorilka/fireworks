
#include "caschannel.hpp"

QIODevice& operator<<(QIODevice& device, const CASChannel& ch)
{
    device.write("channel{");

    device.write(" id: ");
    device.write(QByteArray::number(ch.id()));

    device.write(" id_channel: ");
    device.write(QByteArray::number(ch.idChannel()));

    device.write(" epg_start: ");
    device.write(ch.epgStart().toString(Qt::ISODate).toUtf8());

    device.write(" epg_stop: ");
    device.write(ch.epgStop().toString(Qt::ISODate).toUtf8());

    device.write(" name: ");
    device.write(ch.name().toUtf8());

    device.write(" mrl: ");
    device.write(ch.mrl().toUtf8());

    device.write(" logo: ");
    device.write(ch.logo().toUtf8());

    device.write(" }\r\n");

    return device;
}

const CASChannel& CASChannel::DefaultObject()
{
    static const CASChannel defaultObject;
    return defaultObject;
}

CASChannel::CASChannel(cJSON* object) :
    m_id(0),
    m_idChannel(0)
{
    if(object->type == cJSON_Object)
    {
        cJSON* child = object->child;
        while(child)
        {
            parseJsonValue(child);
            child = child->next;
        }
    }
}

CASChannel::CASChannel() :
    m_id(0),
    m_idChannel(0)
{
}

void CASChannel::parseJsonValue(cJSON* value)
{
    switch(value->type)
    {
    case cJSON_Number :
        if(QString("id") == value->string)
        {
            m_id = value->valueint;
        }
        else if(QString("channel_id") == value->string)
        {
            m_idChannel = value->valueint;
        }
        break;
    case cJSON_String :
        if(QString("name") == value->string)
        {
            m_name = value->valuestring;
        }
        else  if(QString("mrl") == value->string)
        {
            m_mrl = value->valuestring;
        }
        else  if(QString("logo") == value->string)
        {
            m_logo = value->valuestring;
        }
        else if(QString("epg_start") == value->string)
        {
            m_epgStart = QDateTime::fromString(value->valuestring, Qt::ISODate);
        }
        else if(QString("epg_stop") == value->string)
        {
            m_epgStop = QDateTime::fromString(value->valuestring, Qt::ISODate);
        }
        break;
    default :
        break;
    }
}
