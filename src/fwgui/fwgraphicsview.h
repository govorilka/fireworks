#ifndef FIREWORKS_GRAPHICSVIEW_H
#define FIREWORKS_GRAPHICSVIEW_H

#include <QtCore/qobject.h>
#include <QtCore/qrect.h>
#include <QtCore/qhash.h>
#include <QtCore/qplugin.h>

#include <QtGui/qregion.h>

#include "fireworks.h"

#include "fwgui/fwpixmap.h"

#include "fwtypography/fwfont.h"

class FwScene;
class FwMLNode;
class FwRender;
class FwPainter;
class FwKeyPressEvent;
class FwVertexArray;

class FwGraphicsView : public QObject
{
    Q_OBJECT
    typedef QObject BaseClass;

public:
    friend class FwScene;
    friend class FwPrimitive;

    explicit FwGraphicsView(QObject *parent = 0);
    virtual ~FwGraphicsView();

    FwFont font(const FwFontDescription& desc);

    FwPixmap pixmap(const FwPixmapDescription& desc);
    FwPixmap pixmap(const QSize& size);

    inline QSize size() const;
    void setSize(const QSize& size);

    inline QRect rect() const;

    void clearCache();

    FwScene* scene(int id) const;

    inline FwScene* activeScene() const;
    inline void setActiveScene(int id);
    void setActiveScene(FwScene* scene);

    bool event(QEvent *e);

    void render(FwPainter* painter, const QRect& clipRect);

    inline void update();
    inline void update(const QRect& rect);

    virtual FwVertexArray* createVertexArray() const = 0;

protected:
    virtual void keyPressEvent(FwKeyPressEvent* event);

    virtual void invalidateCanvas(const QRegion& region) = 0;

    virtual FwFontData* createFontData(const FwFontDescription& desc) = 0;

    virtual FwPixmapData* createBuffer(const FwPixmapDescription& desc) = 0;
    virtual FwPixmapData* createBuffer(Fw::BufferMode mode, const QSize& size) = 0;

    void invalidateChanges();

private:
    QSize m_size;
    QHash<QString, FwFont> m_fonts;
    QHash<QString, FwPixmap> m_pixmaps;

    FwScene* m_activeScene;
    FwScene* m_prevActiveScene;
    QList<FwScene*> m_scenes;

    bool m_needPostUpdateEvent;
    bool m_needInvalidate;

    QRegion m_dirtyRegion;
};

#include "fwgui/fwgraphicsview_inl.h"

#endif //FIREWORKS_GRAPHICSVIEW_H
