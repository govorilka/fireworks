#include <QtCore/qcoreevent.h>

#include "fwcore/fwml.h"#

#include "fwgui/fwguievent.h"
#include "fwgui/fwscene.h"

#include "fwgui/primitives/fwstringprimitive.h"
#include "fwgui/primitives/fwpixmapprimitive.h"

#include "fwmessagebox.h"

FwMessageBox::FwMessageBox(const QByteArray& name, FwPrimitiveGroup* parent) :
    BaseClass(name, parent),
    m_caption(new FwStringPrimitive("caption", this)),
    m_messageText(new FwStringPrimitive("text", this)),
    m_messageboxbuttons(0)
{
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

void FwMessageBox::setRequest(const FwRequest& request)
{
    m_request = request;
    m_messageText->setString(m_request.text());

    delete m_messageboxbuttons;
    m_messageboxbuttons = new FwMessageBoxButtons(request.answers(), this);

    if(scene()->m_messageBoxTemplate)
    {
        FwMLObject* buttonTemplate = scene()->m_messageBoxTemplate->attribute("buttons")->cast<FwMLObject>();
        if(buttonTemplate)
        {
            m_messageboxbuttons->apply(buttonTemplate);
        }
    }
}

/////////////////////////////////////////////////////////

FwMessageBoxButtons::FwMessageBoxButtons(const QList<FwRequestAnswer> answers, FwMessageBox *parent) :
    BaseClass("buttons", parent)
{
    foreach(FwRequestAnswer answer, answers)
    {
        FwMessageBoxButton* button = new FwMessageBoxButton(answer, this);
        m_buttons.append(button);
    }
}

void FwMessageBoxButtons::apply(FwMLObject *object)
{
    FwMLObject* buttonNode = object->attribute("buttons")->cast<FwMLObject>();
    if(buttonNode)
    {
        foreach(FwMessageBoxButton* button, m_buttons)
        {
            FwMLObject*currentButton = buttonNode->attribute(button->name())->cast<FwMLObject>();
            if(currentButton)
            {
                button->apply(currentButton);
            }
        }
    }
    BaseClass::apply(object);
}

FwMessageBoxButtons::Orientation FwMessageBoxButtons::nameToOrientation(const QByteArray& name, bool* bOk)
{
    if(!name.isEmpty())
    {
        QByteArray lowerName = name.trimmed().toLower();

        Orientation result = O_Invalid;
        if(lowerName == "top")
        {
            result =O_Top;
        }
        else if(lowerName == "middle")
        {
            result =O_Middle;
        }
        else if(lowerName == "bottom")
        {
            result =O_Bottom;
        }
        else if(lowerName == "left")
        {
            result =O_Left;
        }
        else if(lowerName == "center")
        {
            result =O_Center;
        }
        else if(lowerName == "right")
        {
            result =O_Right;
        }

        if(bOk)
        {
            (*bOk) = true;
        }
        return result;

    }

    if(bOk)
    {
        (*bOk) = false;
    }
    return O_Left;
}

QByteArray FwMessageBoxButtons::orientationToName(Orientation value)
{
    switch(value)
    {
      case O_Top:
        return "top";

      case O_Middle:
        return "middle";

      case O_Bottom:
        return "bottom";

      case O_Left:
        return "left";

      case O_Center:
        return "center";

      case O_Right:
        return "right";

      default:
        break;
    }

    return "left";
}

/////////////////////////////////////////////////////////

FwMessageBoxButton::FwMessageBoxButton(const FwRequestAnswer& answer, FwMessageBoxButtons* parent) :
    BaseClass(answer.caption().toUtf8(), parent),
    m_caption(new FwStringPrimitive("caption", this)),
    m_icon(new FwPixmapPrimitive("icon", this)),
    m_result(answer.result())
{
    m_caption->setString(answer.caption());
}
