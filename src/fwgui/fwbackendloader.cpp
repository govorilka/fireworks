#include "fwbackendloader.h"
#include "fwgraphicsview.h"

FwBackendLoader::FwBackendLoader(QObject *parent) :
    BaseClass(parent)
{
}

FwGraphicsView* FwBackendLoader::loadView(const QString& fileName)
{
    setFileName(fileName);
    if(load())
    {
        QObject* object = instance();
        FwGraphicsView* view = qobject_cast<FwGraphicsView*>(object);
        if(view)
        {
            return view;
        }
        delete object;
    }
    return 0;
}
