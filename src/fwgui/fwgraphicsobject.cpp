#include <QtCore/qcoreevent.h>
#include <QtCore/qpropertyanimation.h>
#include <QtCore/qcoreapplication.h>
#include <QtCore/qdebug.h>

#include <QtGui/qevent.h>

#include "fwgraphicsobject.h"
#include "fwscene.h"
#include "fwguievent.h"

FwGraphicsObject::FwGraphicsObject(const QByteArray& name, FwPrimitiveGroup* parent) :
    QObject(),
    BaseClass(name, parent),
    m_showAnimation(0),
    m_hideAnimation(0),
    m_oldSize(0, 0)
{
    if(parent)
    {
        setParent(parent->m_scene);
    }

    m_visible = false;
    visibleOnScreen = false;
}

void FwGraphicsObject::setFont(const FwFont& font)
{
    if(m_font != font)
    {
        m_font = font;
        QEvent event(QEvent::FontChange);
        QCoreApplication::sendEvent(this, &event);
    }
}

void FwGraphicsObject::setShowAnimation(QPropertyAnimation* animation)
{
    if(m_showAnimation != animation)
    {
        if(m_showAnimation)
        {
            m_showAnimation->stop();
            m_showAnimation->disconnect(this);
            this->disconnect(m_showAnimation);
            m_showAnimation->deleteLater();
            m_showAnimation = 0;
        }
        if(animation)
        {
            m_showAnimation = animation;
            m_showAnimation->setParent(this);
            m_showAnimation->setObjectName("showAnimation");
            connect(m_showAnimation, SIGNAL(finished()), this, SLOT(showAnimationFinished()));
        }
    }
}

void FwGraphicsObject::setHideAnimation(QPropertyAnimation* animation)
{
    if(m_hideAnimation != animation)
    {
        if(m_hideAnimation)
        {
            m_hideAnimation->stop();
            m_hideAnimation->disconnect(this);
            this->disconnect(m_hideAnimation);
            m_hideAnimation->deleteLater();
            m_hideAnimation = 0;
        }
        if(animation)
        {
            m_hideAnimation = animation;
            m_hideAnimation->setParent(this);
            m_hideAnimation->setObjectName("hideAnimation");
            connect(m_hideAnimation, SIGNAL(finished()), this, SLOT(hideAnimationFinishel()));
        }
    }
}

void FwGraphicsObject::geometryChanged(const QRect &oldRect, QRect &rect)
{
    if(oldRect.size() != rect.size())
    {
        QResizeEvent resizeEvent(rect.size(), m_oldSize);
        QCoreApplication::sendEvent(this, &resizeEvent);
    }

    FwRectPrimitive::geometryChanged(oldRect, rect);
    m_childrenRect = rect;
}

bool FwGraphicsObject::event(QEvent *e)
{
    if(e->type() == FwGuiEvent::qtTypeID())
    {
        FwGuiEvent* fwEvent = static_cast<FwGuiEvent*>(e);
        switch(fwEvent->eventType())
        {
        case Fw::E_KeyPress:
            keyPressEvent(static_cast<FwKeyPressEvent*>(fwEvent));
            return true;
        default:
            break;
        }
    }

    switch(e->type())
    {
    case QEvent::Resize:
        resizeEvent(static_cast<QResizeEvent*>(e));
        e->accept();
        return true;

    default:
        break;
    }

    return QObject::event(e);
}

void FwGraphicsObject::keyPressEvent(FwKeyPressEvent* event)
{
    Q_UNUSED(event);
}

void FwGraphicsObject::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
}

FwGraphicsObject* FwGraphicsObject::object() const
{
    return const_cast<FwGraphicsObject*>(this);
}
