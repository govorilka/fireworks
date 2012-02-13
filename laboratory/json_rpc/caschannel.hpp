#ifndef CASCHANNEL_HPP
#define CASCHANNEL_HPP

#include <QtCore/QString>
#include <QtCore/QIODevice>

#include "cjson/cJSON.h"

#include "caschannelprogram.hpp"

struct CASChannel
{
public:
    static const CASChannel& DefaultObject();

    explicit CASChannel(cJSON* object);

    inline qint32 id() const;
    inline qint32 idChannel() const;
    inline const QDateTime& epgStart() const;
    inline const QDateTime& epgStop() const;
    inline const QString& name() const;
    inline const QString& mrl() const;
    inline const QString& logo() const;

protected:
    CASChannel();
    void parseJsonValue(cJSON* value);

private:
    qint32 m_id;
    qint32 m_idChannel;

    QDateTime m_epgStart;
    QDateTime m_epgStop;

    QString m_name;
    QString m_mrl;
    QString m_logo;
};

QIODevice& operator<<(QIODevice& device, const CASChannel& ch);

qint32 CASChannel::id() const
{
    return m_id;
}
qint32 CASChannel::idChannel() const
{
    return m_idChannel;
}
const QDateTime& CASChannel::epgStart() const
{
    return m_epgStart;
}
const QDateTime& CASChannel::epgStop() const
{
    return m_epgStop;
}
const QString& CASChannel::name() const
{
    return m_name;
}
const QString& CASChannel::mrl() const
{
    return m_mrl;
}
const QString& CASChannel::logo() const
{
    return m_logo;
}

#endif // CASCHANNEL_HPP
