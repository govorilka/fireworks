
#include <QtCore/QDebug>

#include "globals.hpp"
#include "caschannelprogram.hpp"

QIODevice& operator << (QIODevice& device, const CASChannelProgram& program)
{
    device.write("program{");

    device.write(" start: ");
    device.write(program.start().toString(Qt::ISODate).toUtf8());

    device.write(" stop: ");
    device.write(program.stop().toString(Qt::ISODate).toUtf8());

    device.write(" title: ");
    device.write(program.title().toUtf8());

    device.write(" discription: ");
    device.write(program.discription().toUtf8());

    device.write(" }\r\n");

    return device;
}

CASChannelProgram::CASChannelProgram(cJSON* object)
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

void CASChannelProgram::parseJsonValue(cJSON* value)
{
    if(value->type == cJSON_String)
    {
        if(QString("start") == value->string)
        {
            m_start = QDateTime::fromString(value->valuestring, Qt::ISODate);
            qDebug() << m_start << m_start.utcOffset()/3600;

        }
        else  if(QString("stop") == value->string)
        {
            m_stop = QDateTime::fromString(value->valuestring, Qt::ISODate);
        }
        else  if(QString("title") == value->string)
        {
            m_title = value->valuestring;
        }
        else  if(QString("discription") == value->string)
        {
            m_discription = value->valuestring;
        }
    }
}
