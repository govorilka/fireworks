#ifndef FIREWORKS_GRAPHICSITEM_H
#define FIREWORKS_GRAPHICSITEM_H

#include <QtCore/qrect.h>
#include <QtCore/qvariant.h>

#include "fireworks.h"

#include "fwgui/fwpixmap.h"
#include "fwgui/fwpen.h"

#include "fwtypography/fwfont.h"

class FwScene;
class FwGeometry;
class FwPrimitiveGroup;
class FwSceneGraphicsItem;
class FwWidget;
class FwMLObject;
class FwPainter;
class FwBrush;
class FwPixmapData;
class FwDrawer;
class FwItemView;

class FIREWORKSSHARED_EXPORT FwPrimitive
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
    inline FwDrawer* drawer() const;

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

    void prepareGeometryChanged();
    void update(bool needUpdateBuffer = true);

    inline int zIndex() const;
    void setZIndex(int zIndex);

    inline static bool zIndexLessThan(FwPrimitive* p1, FwPrimitive* p2);

    virtual void apply(FwMLObject* object);

    FwPixmap createPixmap(const QSize& size) const;
    FwPixmap createPixmap(const FwPixmapDescription& desc) const;
    FwPixmap createPixmap(FwMLNode* node) const;

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

    inline FwPen* pen() const;
    void setPen(FwPen* pen);

    inline FwColor penColor() const;
    void setPenColor(const FwColor& color);

    inline QVariant data() const;
    inline void setData(const QVariant& data);

    inline bool isIgnoreParentMargin() const;
    void setIgnoreParentMargin(bool enable);

    inline QPoint mapFromScene(const QPoint& pos) const;
    inline QPoint mapToScene(const QPoint& pos) const;

    inline const QRect& parentRect() const;

    FwPrimitive* primitiveByName(const QByteArray& name);

    virtual void currentChangedEvent(FwItemView* view, bool current);
    virtual void trigger();

protected:

    virtual void paint(FwPainter* painter, const QRect& clipRect) = 0;

    void updateGeometryRect();

    virtual void visibleChangedEvent();
    virtual void geometryChangedEvent(const QRect& oldRect, QRect& rect);
    virtual void boundingRectChangedEvent(QRect& boundingRect);
    virtual void penChangedEvent(FwPen* pen);

    void invalidateCanvas(const QRect& clipRect);

    virtual FwDrawer* createDrawer(const QByteArray& name, FwMLObject* object) const;

    virtual FwPrimitive* primitiveByName(const QList<QByteArray>& name, int firstElement = 0);

private:
    void createNewBuffer();
    void releaseBuffer();
    void updateBuffer();

    FwPrimitiveGroup* m_parent;
    FwScene* m_scene;

    QPoint m_pos;
    QSize m_size;
    FwGeometry* m_geometry;
    FwGeometry* m_parentGeometry;
    bool m_ignoreParentMargin;
    Fw::HorizontalPosition m_hPosition;
    Fw::VerticalPosition m_vPosition;
    QRect m_boundingRect;

    Fw::BufferMode m_bufferMode;
    bool bufferDirty;
    FwPixmapData* m_buffer;

    bool m_visible;
    bool visibleOnScreen;

    int m_zIndex;

    int _startChanged;

    QByteArray m_name;
    QVariant* m_data;

    FwPen* m_pen;

    bool m_contentDirty;

    FwDrawer* m_drawer;
};

#include "fwgui/primitives/fwprimitive_inl.h"

#endif // FIREWORKS_GRAPHICSITEM_H
