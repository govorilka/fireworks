#ifndef FIREWORKS_GRAPHICSITEM_H
#define FIREWORKS_GRAPHICSITEM_H

#include "fireworks.h"

#include "fwgui/fwanchor.h"
#include "fwgui/fwpixmap.h"

#include "fwtypography/fwfont.h"

class FwScene;
class FwAnchor;
class FwCanvas;
class FwPrimitiveGroup;
class FwSceneGraphicsItem;
class FwWidget;
class FwMLObject;
class FwPen;
class FwPainter;
class FwBrush;
class FwPixmapData;

class FwPrimitive
{
public:
    friend class FwPrimitiveGroup;
    friend class FwGraphicsObject;
    friend class FwAnchor;
    friend class FwScene;
    friend class FwWidget;

    FwPrimitive(FwPrimitiveGroup* parent);
    virtual ~FwPrimitive();

    inline FwPrimitiveGroup* parent() const;
    inline FwScene* scene() const;

    inline bool isVisible() const;
    inline bool isVisibleOnScreen() const;
    void setVisible(bool visible);

    inline void show();
    inline void hide();

    Fw::Position position() const;
    void setPosition(Fw::Position position);

    inline Fw::BufferMode bufferMode() const;
    void setBufferMode(Fw::BufferMode mode);

    inline QPoint xy() const;
    inline void setXY(int x, int y);
    inline void setXY(const QPoint& pos);

    inline QRect rect() const;
    inline void setRect(const QRect& rect);
    inline QRect boundingRect() const;

    inline int x() const;
    inline void setX(int x);
    inline int y() const;
    inline void setY(int y);

    inline QSize size() const;
    inline void setSize(int w, int h);
    inline void setSize(const QSize& size);

    inline void prepareGeometryChanged();
    void invalidate();
    void update(bool needUpdateBuffer = true);

    inline int zIndex() const;
    void setZIndex(int zIndex);

    inline static bool zIndexLessThan(FwPrimitive* p1, FwPrimitive* p2);

    inline FwAnchor* anchor() const;
    inline void linkAnchor(FwAnchor *anchor);

    virtual void apply(FwMLObject* object);

    FwPixmap createPixmap(const QSize& size);
    FwPixmap createPixmap(const FwPixmapDescription& desc);
    FwPixmap createPixmap(FwMLNode* node);

    FwFont createFont(const FwFontDescription& desc);
    FwFont createFont(FwMLObject* object, const QByteArray& attribute);

    FwPen* createPen(FwMLObject* object, const QByteArray& penAttr);

    FwBrush* createBrush(FwMLObject* object);

protected:
    virtual QRect updateGeometry(const QRect& rect);

    virtual void paint(FwPainter* painter, const QRect& clipRect) = 0;

    virtual void visibleChangedEvent();

private:
    void createNewBuffer();
    void releaseBuffer();
    void updateBuffer();

    void setGeometryRect(const QRect& rect);

    FwPrimitiveGroup* m_parent;
    FwScene* m_scene;

    int m_x;
    int m_y;
    FwAnchor* m_anchor;
    FwGeometry* m_geometry;

    QRect m_boundingRect;
    bool m_boundingRectDirty;

    Fw::BufferMode m_bufferMode;
    bool bufferDirty;
    FwPixmapData* m_buffer;

    bool m_visible;
    bool visibleOnScreen;

    int m_zIndex;

    int _startChanged;
};

#include "fwprimitives/fwprimitive_inl.h"

#endif // FIREWORKS_GRAPHICSITEM_H
