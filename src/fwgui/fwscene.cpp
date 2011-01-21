#include <QtCore/qcoreevent.h>
#include <QtCore/qcoreapplication.h>
#include <QtCore/qdebug.h>

#include <QtGui/qevent.h>

#include "fwscene.h"
#include "fwcanvas.h"
#include "fwwidget.h"
#include "fwgraphicsview.h"

FwScene::FwScene(int id, FwGraphicsView* view) :
    BaseClass(0, view),
    m_view(view),
    m_id(id),
    needPostUpdateEvent(true)
{
    Q_ASSERT(!m_view->scene(id));
    m_view->m_scenes.append(this);
    m_scene = this;
}

FwScene::~FwScene()
{
    if(m_view)
    {
        if(m_view->m_activeScene == this)
        {
            m_view->setActiveScene(0);
        }
        m_view->m_scenes.removeAll(this);
    }

    foreach(FwWidget* widget, m_widgets)
    {
        delete widget;
    }

    m_widgets.clear();
}

void FwScene::showEvent(FwSceneShowEvent* event)
{
    Q_UNUSED(event);
    setVisible(true);
}

void FwScene::hideEvent(FwSceneHideEvent* event)
{
    Q_UNUSED(event);
    setVisible(false);
}

void FwScene::showAnimationFinished()
{

}

void FwScene::hideAnimationFinished()
{
}

void FwScene::update(const QRect& rect)
{
    if(m_view && m_view->m_activeScene == this)
    {
        QRect displayRect = this->rect().intersected(rect);
        if(!displayRect.isEmpty())
        {
            m_dirtyRegion = m_dirtyRegion.united(displayRect);
            if(needPostUpdateEvent)
            {
                QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateLater));
                needPostUpdateEvent = false;
            }
        }
    }
}

bool FwScene::event(QEvent * e)
{
    if(e->type() == FwSceneShowEvent::typeID())
    {
        FwSceneShowEvent* showEvent = static_cast<FwSceneShowEvent*>(e);
        showEventProcessed(showEvent);
        e->accept();
        return true;
    }
    else if(e->type() == FwSceneHideEvent::typeID())
    {
        FwSceneHideEvent* hideEvent = static_cast<FwSceneHideEvent*>(e);
        hideEventProcessed(hideEvent);
        e->accept();
        return true;
    }
    else
    {
        switch(e->type())
        {
        case QEvent::UpdateLater:
            invalidateDirtyRegion();
            e->accept();
            return true;

        default:
            break;
        }
    }

    return BaseClass::event(e);
}

/*!
Перерисовывает регион, заданный переменной m_dirtyRegion. После выполнения,
функция очищает значение m_dirtyRegion.
\note Рисование происходит вызовом функции
*/
void FwScene::invalidateDirtyRegion()
{
    if(!m_dirtyRegion.isEmpty())
    {
        invalidateRegion(m_dirtyRegion);
        m_dirtyRegion = QRegion();
        needPostUpdateEvent = true;
    }
}

/*!
Перерисовает заданный регион в окне
\param rgn Регион, который необходимо перерисовать
\sa FwScene::invalidateDirtyRegion()
*/
void FwScene::invalidateRegion(const QRegion& rgn)
{
    if(m_boundingRectDirty)
    {
        m_boundingRect = updateGeometry(rect());
        m_boundingRectDirty = false;
    }

    foreach(QRect rect, rgn.rects())
    {
        FwRender* render = m_view->createRender(rect);
        if(render)
        {
            FwCanvas canvas(render);
            canvas.setRect(rect);
            canvas.setColor(FwColor(0x00, 0x00, 0x00, 0x00));
            canvas.drawFillRect(rect);
            paint(&canvas);
            canvas.flip(rect);
        }
    }
}

FwFont FwScene::font(const FwFontDescription& description)
{
    return m_view->font(description);
}

void FwScene::showEventProcessed(FwSceneShowEvent* e)
{
    setVisible(true);
    showEvent(e);
}

void FwScene::hideEventProcessed(FwSceneHideEvent* e)
{
    setVisible(false);
    hideEvent(e);
}

////////////////////////////////////////////////////////////////////////////////

FwSceneShowEvent::FwSceneShowEvent(FwScene* previous) :
    BaseClass(static_cast<QEvent::Type>(typeID())),
    m_previous(previous)
{
}

int FwSceneShowEvent::typeID()
{
    static int _typeID = QEvent::registerEventType();
    return _typeID;
}

////////////////////////////////////////////////////////////////////////////////

FwSceneHideEvent::FwSceneHideEvent(FwScene* next) :
    BaseClass(static_cast<QEvent::Type>(typeID())),
    m_next(next)
{
}

int FwSceneHideEvent::typeID()
{
    static int _typeID = QEvent::registerEventType();
    return _typeID;
}
