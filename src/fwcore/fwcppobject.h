#ifndef FIREWORKS_CPPOBJECT_H
#define FIREWORKS_CPPOBJECT_H

#include "fireworks.h"

class FwMLObject;

class FIREWORKSSHARED_EXPORT FwCPPObject
{
public:
    explicit FwCPPObject(const QByteArray& name);

    inline const QByteArray& name() const;
    inline void setname(QByteArray& name);

    bool loadFile(const QString& fileName);

    virtual bool loadData(FwMLObject* object) = 0;
    virtual void resetData();

private:
    QByteArray m_name;
};


const QByteArray& FwCPPObject::name() const
{
    return m_name;
}

void FwCPPObject::setname(QByteArray& name)
{
    m_name = name;
}

#endif // FIREWORKSCPPOBJECT_H
