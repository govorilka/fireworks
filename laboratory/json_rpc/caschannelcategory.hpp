
#ifndef CASCHANNELCATEGORY_HPP
#define CASCHANNELCATEGORY_HPP

#include <QtCore/QString>
#include <QtCore/QSet>
#include <QtCore/QIODevice>

#include "cjson/cJSON.h"

struct CASChannelCategory
{
public :
    explicit CASChannelCategory(cJSON* object);

    inline qint32 id() const;
    inline const QString& name() const;
    inline const QString& logo() const;
    inline const QSet<qint32>& idChannels() const;

protected:
    void parseJsonValue(cJSON* value);
    void addItem(cJSON* value);

private :
    qint32 m_id;
    QString m_name;
    QString m_logo;

    QSet<qint32> m_idChannels;
};

QIODevice& operator << (QIODevice& device, const CASChannelCategory& ch);

qint32 CASChannelCategory::id() const
{
    return m_id;
}
const QString& CASChannelCategory::name() const
{
    return m_name;
}
const QString& CASChannelCategory::logo() const
{
    return m_logo;
}

const QSet<qint32>& CASChannelCategory::idChannels() const
{
    return m_idChannels;
}

#endif // CASCHANNELCATEGORY_HPP
