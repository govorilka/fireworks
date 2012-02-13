#ifndef CASCHANNELPROGRAM_HPP
#define CASCHANNELPROGRAM_HPP

#include <QtCore/QString>
#include <QtCore/QIODevice>
#include <QtCore/QDateTime>

#include "cjson/cJSON.h"

struct CASChannelProgram
{
public:
    explicit CASChannelProgram(cJSON* object);

    inline const QDateTime& start() const;
    inline const QDateTime& stop() const;
    inline const QString& title() const;
    inline const QString& discription() const;

protected:
    void parseJsonValue(cJSON* value);

private:
    QDateTime m_start;
    QDateTime m_stop;

    QString m_title;
    QString m_discription;
};

QIODevice& operator << (QIODevice& device, const CASChannelProgram& program);

const QDateTime& CASChannelProgram::start() const
{
    return m_start;
}

const QDateTime& CASChannelProgram::stop() const
{
    return m_stop;
}

const QString& CASChannelProgram::title() const
{
    return m_title;
}

const QString& CASChannelProgram::discription() const
{
    return m_discription;
}

#endif // CASCHANNELPROGRAM_HPP
