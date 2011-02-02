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

    inline void setActiveScene(int id);
    void setActiveScene(FwScene* scene);

    bool event(QEvent *e);

protected:
    virtual void keyPressEvent(FwKeyPressEvent* event);

    virtual void updateCanvas(const QRect& rect) = 0;

    virtual FwFontData* createFontData(const FwFontDescription& desc) = 0;

    virtual FwPixmapData* createBuffer(const FwPixmapDescription& desc) = 0;
    virtual FwPixmapData* createBuffer(Fw::BufferMode mode, const QSize& size) = 0;
    virtual FwRender* createRender(const QRect& rect) = 0;

    void render(FwPainter* painter);

private:
    QSize m_size;
    QHash<QString, FwFont> m_fonts;
    QHash<QString, FwPixmap> m_pixmaps;

    FwScene* m_activeScene;
    FwScene* m_prevActiveScene;
    QList<FwScene*> m_scenes;
    bool m_needPostUpdateEvent;
    QRegion m_dirtyRegion;
};

Q_DECLARE_INTERFACE(FwGraphicsView, "com.fireworks.fwgraphicsview/0.1")


#include "fwgui/fwgraphicsview_inl.h"

#endif //FIREWORKS_GRAPHICSVIEW_H
