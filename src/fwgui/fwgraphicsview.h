#ifndef FIREWORKS_GRAPHICSVIEW_H
#define FIREWORKS_GRAPHICSVIEW_H

#include <QtCore/qobject.h>
#include <QtCore/qrect.h>
#include <QtCore/qhash.h>
#include <QtCore/qplugin.h>
#include <QtCore/qqueue.h>

#include "fireworks.h"

#include "fwcore/fwcppobject.h"

#include "fwgui/fwpixmap.h"
#include "fwgui/fwregion.h"

#include "fwtypography/fwfont.h"

class FwScene;
class FwMLNode;
class FwRender;
class FwPainter;
class FwKeyPressEvent;
class FwVertexArray;
class FwRequest;
class FwMLEngine;
class FwImageLibrary;

class FIREWORKSSHARED_EXPORT FwGraphicsView : public QObject, public FwCPPObject
{
    Q_OBJECT
    typedef FwCPPObject BaseClass;

public:
    friend class FwScene;
    friend class FwPrimitive;
    friend class FwPrimitiveGroup;

    explicit FwGraphicsView(QObject *parent = 0);
    virtual ~FwGraphicsView();

    FwFont font(const FwFontDescription& desc);

    FwPixmap pixmap(const FwPixmapDescription& desc);
    FwPixmap pixmap(const QSize& size);

    inline QSize size() const;
    void setSize(const QSize& size);

    inline QRect rect() const;

    void clearCache();

    FwScene* scene(const QByteArray& name) const;

    inline FwScene* activeScene() const;
    inline void setActiveScene(const QByteArray& name);
    void setActiveScene(FwScene* scene);

    inline FwScene* prevActiveScene() const;

    bool event(QEvent *e);

    void render(FwPainter* painter, const QRect& clipRect);

    inline void update();
    inline void update(const QRect& rect);

    virtual FwVertexArray* createVertexArray() const = 0;

    void postRequest(const FwRequest& request);

    inline FwImageLibrary* imageLibrary() const;

    bool loadData(FwMLEngine* engine);
    bool loadData(FwMLObject* object);

    virtual bool init() = 0;

protected:
    virtual void invalidateCanvas(const FwRegion& region) = 0;

    virtual FwFontData* createFontData(const FwFontDescription& desc) = 0;

    virtual FwPixmapData* createBuffer(const FwPixmapDescription& desc) = 0;
    virtual FwPixmapData* createBuffer(Fw::BufferMode mode, const QSize& size) = 0;

    void invalidateChanges();

    inline void postUpdateEvent();

    virtual void keyEvent(FwKeyPressEvent* event);

private:
    QSize m_size;
    QHash<QString, FwFont> m_fonts;
    QHash<QString, FwPixmap> m_pixmaps;

    FwScene* m_activeScene;
    FwScene* m_prevActiveScene;
    QList<FwScene*> m_scenes;
    QQueue<FwRequest> m_requests;

    bool m_needPostUpdateEvent;

    FwRegion* m_dirtyRegion;

    FwImageLibrary* m_imageLibrary;
};

#include "fwgui/fwgraphicsview_inl.h"

#endif //FIREWORKS_GRAPHICSVIEW_H
