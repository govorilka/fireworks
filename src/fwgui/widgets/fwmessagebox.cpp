#include <QtCore/qcoreevent.h>

#include "fwmessagebox.h"

#include "fwgui/fwguievent.h"
#include "fwgui/fwscene.h"

#include "fwcore/fwml.h"

FwMessageBox::FwMessageBox(const QByteArray& name, FwPrimitiveGroup* parent) :
    BaseClass(name, parent),
    m_caption(new FwStringPrimitive("caption", this))
{
}

void FwMessageBox::apply(FwMLObject *object)
{
    if(object)
    {
        FwMLNode* buttonNode = object->attribute("button");
        if(buttonNode)
        {

        }
    }
    BaseClass::apply(object);
}

void FwMessageBox::keyPressEvent(FwKeyPressEvent *event)
{
    if(acceptKey(event->key()))
    {
        event->accept();
        return;
    }
    BaseClass::keyPressEvent(event);
}

bool FwMessageBox::accept(int result)
{
    if(result)
    {
        FwScene* scene = this->scene();
        if(scene->m_messageBox == this)
        {
            scene->m_messageBox = 0;
            hide();
            deleteLater();
        }

        if(!m_request.isNull())
        {
            m_request.postAnswer(result);
        }
        return true;
    }

    return false;
}

bool FwMessageBox::acceptKey(int key)
{
    return accept(m_request.result(key));
}
