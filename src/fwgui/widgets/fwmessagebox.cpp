#include <QtCore/qcoreevent.h>

#include "fwcore/fwml.h"

#include "fwgui/fwguievent.h"
#include "fwgui/fwscene.h"

#include "fwgui/primitives/fwstringprimitive.h"
#include "fwgui/primitives/fwpixmapprimitive.h"

#include "fwmessagebox.h"

FwMessageBox::FwMessageBox(const QByteArray& name, FwPrimitiveGroup* parent) :
    BaseClass(name, parent),
    m_caption(new FwStringPrimitive("caption", this)),
    m_messageText(new FwStringPrimitive("text", this)),
    m_buttonBox(0)
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

    delete m_buttonBox;
    m_buttonBox = new FwButtonsBox(request.answers(), this);

    if(scene()->m_messageBoxTemplate)
    {
        apply(scene()->m_messageBoxTemplate);
    }
}

/////////////////////////////////////////////////////////

FwButtonsBox::FwButtonsBox(const QList<FwRequestAnswer> answers, FwMessageBox *parent) :
    BaseClass("buttonBox", parent),
    m_orientation(FwButtonsBox::O_Left),
    m_margin(0)
{
    foreach(FwRequestAnswer answer, answers)
    {
        FwMessageBoxButton* button = new FwMessageBoxButton(answer, this);
        m_buttons.append(button);
    }
}

void FwButtonsBox::apply(FwMLObject *object)
{
    FwMLObject* buttonNode = object->attribute("button")->cast<FwMLObject>();
    if(buttonNode)
    {
        foreach(FwMessageBoxButton* button, m_buttons)
        {
            button->apply(buttonNode);
        }
    }

    bool bOk = true;

    FwMLNode* marginValue = object->attribute("margin");
    if(marginValue)
    {
        m_margin = marginValue->toInt(&bOk);
    }

    FwMLString* buttonOrientation = object->attribute("orientation")->cast<FwMLString>();
    if(buttonOrientation)
    {
        Orientation value = nameToOrientation(buttonOrientation->value(), &bOk);
        if(bOk)
        {
            setOrientation(value, false);
        }
    }

    BaseClass::apply(object);

    updateButtonsPos();
}

FwButtonsBox::Orientation FwButtonsBox::nameToOrientation(const QByteArray& name, bool* bOk)
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

QByteArray FwButtonsBox::orientationToName(Orientation value)
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

void FwButtonsBox::setOrientation(Orientation value, bool needUpdatePos)
{
    if(m_orientation != value)
    {
        m_orientation = value;
        if(needUpdatePos)
        {
            updateButtonsPos();
        }
    }
}

void FwButtonsBox::updateButtonsPos()
{
    switch(m_orientation)
    {
    case FwButtonsBox::O_Left:
        updateHorizontalButtonsPos(0);
        return;

    case FwButtonsBox::O_Center:
        updateHorizontalButtonsPos((width() - totalWidth())/2);
        return;

    case FwButtonsBox::O_Right:
        updateHorizontalButtonsPos(width() - totalWidth());
        return;

    case FwButtonsBox::O_Top:
        updateVerticalButtonsPos(0);
        return;

    case FwButtonsBox::O_Middle:
        updateVerticalButtonsPos((height() - totalHeight())/2);
        return;

    case FwButtonsBox::O_Bottom:
        updateVerticalButtonsPos(height() - totalHeight());
        return;

    default:
        Q_ASSERT(false);
        break;
    }
}

void FwButtonsBox::updateHorizontalButtonsPos(int x)
{
    int h = height();
    foreach(FwMessageBoxButton* button, m_buttons)
    {
        button->setPos(x, (h - button->height()) / 2);
        x += (button->geometry()->rect().width() + m_margin);
    }
}

void FwButtonsBox::updateVerticalButtonsPos(int y)
{
    int h = width();
    foreach(FwMessageBoxButton* button, m_buttons)
    {
        button->setPos((h - button->width()) / 2, y);
        y += (button->geometry()->rect().height() + m_margin);
    }
}

int FwButtonsBox::totalWidth()
{
    int totalwidth = 0;
    foreach(FwMessageBoxButton* button, m_buttons)
    {
        totalwidth += button->width() + m_margin;
    }
    totalwidth -= m_margin;
    return totalwidth;
}

int FwButtonsBox::totalHeight()
{
    int totalheight = 0;
    foreach(FwMessageBoxButton* button, m_buttons)
    {
        totalheight += button->height() + m_margin;
    }
    totalheight -= m_margin;
    return totalheight;
}

/////////////////////////////////////////////////////////

FwMessageBoxButton::FwMessageBoxButton(const FwRequestAnswer& answer, FwButtonsBox* parent) :
    BaseClass(answer.name(), parent),
    m_caption(new FwStringPrimitive("caption", this)),
    m_icon(new FwPixmapPrimitive("icon", this))
{
    m_caption->setString(answer.caption());
}