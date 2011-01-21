#include <QtCore/qcoreevent.h>
#include <QtCore/qpropertyanimation.h>
#include <QtCore/qcoreapplication.h>
#include <QtCore/qdebug.h>

#include <QtGui/qevent.h>

#include "fwgraphicsobject.h"

FwGraphicsObject::FwGraphicsObject(FwPrimitiveGroup* root, QObject *parent) :
    QObject(parent),
    BaseClass(root),
    m_showAnimation(0),
    m_hideAnimation(0),
    m_oldSize(0, 0)
{
    delete m_anchor;
    m_anchor = 0;

    m_geometry->setRect(0, 0, 0, 0);
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

QRect FwGraphicsObject::updateGeometry(const QRect &rect)
{
    if(m_oldSize != rect.size())
    {
        QResizeEvent resizeEvent(rect.size(), m_oldSize);
        QCoreApplication::sendEvent(this, &resizeEvent);
        m_oldSize = rect.size();
    }
    return BaseClass::updateGeometry(rect);
}

bool FwGraphicsObject::event(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::Resize:
        resizeEvent(static_cast<QResizeEvent*>(e));
        e->accept();
        return true;

    case QEvent::KeyPress:
        keyPressEvent(static_cast<QKeyEvent*>(e));
        e->accept();
        return true;

    default:
        break;
    }

    return QObject::event(e);
}

void FwGraphicsObject::keyPressEvent(QKeyEvent* event)
{
    Q_UNUSED(event);
}

void FwGraphicsObject::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
}

