#include <QtCore/qcoreapplication.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qdebug.h>

#include "fwgraphicsview.h"
#include "fwscene.h"
#include "fwpainter.h"
#include "fwguievent.h"

#include "fwcore/fwml.h"

FwGraphicsView::FwGraphicsView(QObject *parent) :
    BaseClass(parent),
    m_activeScene(0),
    m_prevActiveScene(0),
    m_needPostUpdateEvent(true)
{
}

FwGraphicsView::~FwGraphicsView()
{
    foreach(FwScene* scene, m_scenes)
    {
        scene->m_view = 0;
        delete scene;
    }
    m_scenes.clear();

    clearCache();
}

FwFont FwGraphicsView::font(const FwFontDescription& desc)
{
    if(!desc.isNull())
    {
        QString fontKey = desc.key();
        if(m_fonts.contains(fontKey))
        {
            return m_fonts.value(fontKey);
        }
        else
        {
            FwFontData* data = createFontData(desc);
            if(data)
            {
                FwFont font(data);
                m_fonts.insert(fontKey, font);
                return font;
            }
        }
    }

    return FwFont();
}

FwPixmap FwGraphicsView::pixmap(const FwPixmapDescription& desc)
{
    if(!desc.isNull())
    {
        QString pixmapKey = desc.key();
        if(m_pixmaps.contains(pixmapKey))
        {
            return m_pixmaps.value(pixmapKey);
        }
        else
        {
            FwPixmapData* data = createBuffer(desc);
            if(data)
            {
                data->setBlendingEnable(desc.blending());

                FwPixmap pixmap(data);
                m_pixmaps.insert(pixmapKey, pixmap);
                return pixmap;
            }
        }
    }
    return FwPixmap();
}

FwPixmap FwGraphicsView::pixmap(const QSize& size)
{
    if(size.width() != 0 && size.height() != 0)
    {
        FwPixmapData* data = createBuffer(Fw::BM_SystemDefault, size);
        if(data)
        {
            return FwPixmap(data);
        }
    }
    return FwPixmap();
}

void FwGraphicsView::setSize(const QSize& size)
{
    if(m_size != size)
    {
        m_size = size;
        if(m_activeScene)
        {
            m_activeScene->setSize(m_size);
        }
        update();
    }
}

void FwGraphicsView::clearCache()
{
    foreach(FwFont font, m_fonts.values())
    {
        font.release();
    }
    m_fonts.clear();

    foreach(FwPixmap pixmap, m_pixmaps.values())
    {
        pixmap.release();
    }
    m_pixmaps.clear();
}

FwScene* FwGraphicsView::scene(int id) const
{
    foreach(FwScene* scene, m_scenes)
    {
        if(scene->id() == id)
        {
            return scene;
        }
    }
    return 0;
}

void FwGraphicsView::setActiveScene(FwScene* scene)
{
    if(m_activeScene != scene)
    {
        Q_ASSERT(scene->m_view == this);

        m_prevActiveScene = m_activeScene;
        if(m_prevActiveScene)
        {
            m_activeScene = 0;
            FwSceneHideEvent sceneHideEvent(scene);
            QCoreApplication::sendEvent(m_prevActiveScene, &sceneHideEvent);
        }

        if(scene)
        {
            m_activeScene = scene;
            if(m_activeScene->size() != m_size)
            {
                m_activeScene->setSize(m_size);
            }

            FwSceneShowEvent sceneShowEvent(m_prevActiveScene);
            QCoreApplication::sendEvent(m_activeScene, &sceneShowEvent);
        }

        update();
    }
}

bool FwGraphicsView::event(QEvent *e)
{
    if(e->type() == QEvent::UpdateLater)
    {
        invalidateChanges();
        e->accept();
        return true;
    }
    else if(e->type() == FwGuiEvent::qtTypeID())
    {
        FwGuiEvent* fwEvent = static_cast<FwGuiEvent*>(e);
        switch(fwEvent->eventType())
        {
        case Fw::E_KeyPress:
            if(m_activeScene)
            {
                return m_activeScene->keyEventProccessed(static_cast<FwKeyPressEvent*>(fwEvent));
            }
            break;

        default:
            break;
        }
    }

    return BaseClass::event(e);
}

void FwGraphicsView::render(FwPainter* painter, const QRect& clipRect)
{
    if(m_activeScene)
    {
        m_activeScene->paint(painter, clipRect);
    }
}

void FwGraphicsView::invalidateChanges()
{
    if(m_activeScene)
    {
        m_activeScene->invalidate();
    }

    m_dirtyRegion.validation();
    if(!m_dirtyRegion.isEmpty())
    {
        invalidateCanvas(m_dirtyRegion);
        m_dirtyRegion.clear();
    }

    m_needPostUpdateEvent = true;
}
