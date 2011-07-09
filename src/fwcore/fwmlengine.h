#ifndef FIREWORKS_MLENGINE_H
#define FIREWORKS_MLENGINE_H

#include <QObject>

class FwMLEngine : public QObject
{
    Q_OBJECT
    typedef QObject BaseClass;

public:
    explicit FwMLEngine(QObject *parent = 0);



};

#endif // FIREWORKS_MLENGINE_H
