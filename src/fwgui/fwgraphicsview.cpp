#include <QtCore/qcoreapplication.h>
#include <QtGui/qevent.h>

#include "fwgraphicsview.h"
#include "fwscene.h"
#include "fwrender.h"

#include "fwcore/fwml.h"

FwGraphicsView::FwGraphicsView(QObject *parent) :
    BaseClass(parent),
    m_activeScene(0),
    m_prevActiveScene(0)
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

FwFont FwGraphicsView::font(FwMLNode* node)
{
    FwFontDescription desc;
    FwMLObject* object = node->cast<FwMLObject>();
    if(object && desc.apply(object))
    {
        return font(desc);
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
    Q_ASSERT(scene->m_view == this);

    m_prevActiveScene = m_activeScene;
    if(m_activeScene)
    {
        m_activeScene = 0;
        if(m_prevActiveScene)
        {
            FwSceneHideEvent sceneHideEvent(scene);
            QCoreApplication::sendEvent(m_prevActiveScene, &sceneHideEvent);
        }
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

        m_activeScene->invalidate();
    }
    else
    {
        clearBackground();
    }
}

bool FwGraphicsView::event(QEvent *e)
{
    if(e->type() == QEvent::KeyPress)
    {
        keyEvent(static_cast<QKeyEvent*>(e));
        e->accept();
        return true;
    }

    return BaseClass::event(e);
}

void FwGraphicsView::keyEvent(QKeyEvent* event)
{
    if(m_activeScene)
    {
        QCoreApplication::sendEvent(m_activeScene, event);
    }
}

void FwGraphicsView::invalidateView(const QRect& rect, FwCanvas* canvas)
{
    if(m_activeScene)
    {
        m_activeScene->paint(canvas);
    }
}
