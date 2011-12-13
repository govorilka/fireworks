#ifndef FIREWORKS_CPPOBJECT_H
#define FIREWORKS_CPPOBJECT_H

#include "fireworks.h"

#include "fw/exception.hpp"

class FwMLObject;

class FIREWORKSSHARED_EXPORT FwCPPObject
{
public:
    explicit FwCPPObject(const QByteArray& name);

    inline const QByteArray& name() const;
    inline void setName(QByteArray& name);

    void loadFile(const QString& fileName) throw(Fw::Exception&);

    virtual bool loadData(FwMLObject* object) = 0;
    virtual void resetData();

private:
    QByteArray m_name;
};


const QByteArray& FwCPPObject::name() const
{
    return m_name;
}

void FwCPPObject::setName(QByteArray& name)
{
    m_name = name;
}


#endif // FIREWORKSCPPOBJECT_H
