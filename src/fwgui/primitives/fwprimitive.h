#ifndef FIREWORKS_GRAPHICSITEM_H
#define FIREWORKS_GRAPHICSITEM_H

#include <QtCore/qrect.h>

#include "fireworks.h"

#include "fwgui/fwpixmap.h"

#include "fwtypography/fwfont.h"

class FwScene;
class FwGeometry;
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
    friend class FwGeometry;
    friend class FwScene;
    friend class FwWidget;

    FwPrimitive(const QByteArray& name, FwPrimitiveGroup* parent);
    virtual ~FwPrimitive();

    inline FwPrimitiveGroup* parent() const;
    inline FwScene* scene() const;

    inline bool isVisible() const;
    inline bool isVisibleOnScreen() const;
    void setVisible(bool visible);

    inline void show();
    inline void hide();

    inline Fw::BufferMode bufferMode() const;
    void setBufferMode(Fw::BufferMode mode);

    inline QPoint pos() const;
    inline void setPos(int x, int y);
    void setPos(const QPoint& pos);

    inline int x() const;
    inline void setX(int x);
    inline int y() const;
    inline void setY(int y);

    inline QSize size() const;
    inline void setSize(int w, int h);
    void setSize(const QSize& size);

    inline int width() const;
    inline void setWidth(int width);
    inline int height() const;
    inline void setHeight(int height);

    inline QRect rect() const;
    void setRect(const QRect& rect);

    inline FwGeometry* geometry() const;

    inline QRect geometryRect() const;
    inline void setGeometryRect(QRect rect);

    inline void prepareGeometryChanged();
    void invalidate();
    void update(bool needUpdateBuffer = true);

    inline int zIndex() const;
    void setZIndex(int zIndex);

    inline static bool zIndexLessThan(FwPrimitive* p1, FwPrimitive* p2);

    virtual void apply(FwMLObject* object);

    FwPixmap createPixmap(const QSize& size);
    FwPixmap createPixmap(const FwPixmapDescription& desc);
    FwPixmap createPixmap(FwMLNode* node);

    FwFont createFont(const FwFontDescription& desc);
    FwFont createFont(FwMLObject* object, const QByteArray& attribute);

    FwPen* createPen(FwMLObject* object, const QByteArray& penAttr);

    FwBrush* createBrush(FwMLObject* object);

    bool loadPos(FwMLObject* object, QPoint* pos);
    bool loadSize(FwMLObject* object,QSize* size);
    bool loadRect(FwMLObject* object, QRect* rect);

    inline QByteArray name() const;
    inline void setName(const QByteArray& name);

    inline bool isLinked() const;
    void link(FwGeometry* parentGeometry);

    inline Fw::HorizontalPosition hPosition() const;
    inline void setHPosition(Fw::HorizontalPosition position);

    inline Fw::VerticalPosition vPosition() const;
    inline void setVPosition(Fw::VerticalPosition position);

    void setPosition(Fw::HorizontalPosition hPosition, Fw::VerticalPosition vPosition);

protected:
    virtual void geometryChanged(const QRect& oldRect, QRect& rect);

    virtual void paint(FwPainter* painter, const QRect& clipRect) = 0;

    void updateGeometryRect(const QRect& parentRect, QRect currentRect);

    virtual void visibleChangedEvent();

private:
    void createNewBuffer();
    void releaseBuffer();
    void updateBuffer();

    FwPrimitiveGroup* m_parent;
    FwScene* m_scene;

    QPoint m_pos;
    FwGeometry* m_geometry;
    Fw::HorizontalPosition m_hPosition;
    Fw::VerticalPosition m_vPosition;
    FwGeometry* m_parentGeometry;

    Fw::BufferMode m_bufferMode;
    bool bufferDirty;
    FwPixmapData* m_buffer;

    bool m_visible;
    bool visibleOnScreen;

    int m_zIndex;

    int _startChanged;

    QByteArray m_name;
};

#include "fwgui/primitives/fwprimitive_inl.h"

#endif // FIREWORKS_GRAPHICSITEM_H
