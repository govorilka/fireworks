
#include "caschannelcategory.hpp"

QIODevice& operator << (QIODevice& device, const CASChannelCategory& chCat)
{
    device.write("category{");

    device.write(" id: ");
    device.write(QByteArray::number(chCat.id()));

    device.write(" name: ");
    device.write(chCat.name().toUtf8());

    device.write(" logo: ");
    device.write(chCat.logo().toUtf8());

    device.write(" channels_id: ");

    foreach(qint32 idChannel, chCat.idChannels())
    {
        device.write(QByteArray::number(idChannel));
        device.write(", ");
    }

    device.write(" }\r\n");

    return device;
}

CASChannelCategory::CASChannelCategory(cJSON* object) :
    m_id(0)
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

void CASChannelCategory::parseJsonValue(cJSON* value)
{
    switch(value->type)
    {
    case cJSON_Number :
        if(QString("id") == value->string)
        {
            m_id = value->valueint;
        }
        break;
    case cJSON_String :
        if(QString("name") == value->string)
        {
            m_name = value->valuestring;
        }
        else  if(QString("logo") == value->string)
        {
            m_logo = value->valuestring;
        }
        break;
    case cJSON_Array :
        if(QString("items") == value->string)
        {
            cJSON* child = value->child;
            while(child)
            {
                addItem(child);
                child = child->next;
            }
        }
        break;
    default :
        break;
    }
}

void CASChannelCategory::addItem(cJSON* value)
{
    if(value->type == cJSON_Number)
    {
        m_idChannels.insert(value->valueint);
    }
}
