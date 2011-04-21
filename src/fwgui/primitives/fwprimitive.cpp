#include <QtCore/qdebug.h>

#include "fwprimitive.h"
#include "fwdrawer.h"

#include "fwgui/fwscene.h"
#include "fwgui/fwgraphicsview.h"
#include "fwgui/fwpixmap.h"
#include "fwgui/fwpainter.h"
#include "fwgui/fwpen.h"

#include "fwgui/widgets/fwwidget.h"

#include "fwcore/fwml.h"

FwPrimitive::FwPrimitive(const QByteArray& name, FwPrimitiveGroup* parent) :
    m_parent(parent),
    m_scene(0),
    m_pos(0, 0),
    m_size(0, 0),
    m_geometry(new FwGeometry()),
    m_hPosition(Fw::HP_Left),
    m_vPosition(Fw::VP_Top),
    m_parentGeometry(0),
    m_ignoreParentMargin(false),
    m_bufferMode(Fw::BM_NoBuffer),
    bufferDirty(false),
    m_buffer(0),
    m_visible(true),
    visibleOnScreen(false),
    m_zIndex(0),
    _startChanged(0),
    m_name(name),
    m_data(0),
    m_pen(0),
    m_contentDirty(false),
    m_drawer(0)
{
    if(m_parent)
    {
        visibleOnScreen = m_parent->visibleOnScreen;
        m_scene = m_parent->m_scene;
        m_parent->m_primitives.append(this);
        link(m_parent->m_geometry);
        m_parent->updateChildrenRect();
        m_parent->sortZIndex();
    }
}

FwPrimitive::~FwPrimitive()
{
    if(m_parent)
    {
        m_parent->m_primitives.remove(m_parent->m_primitives.indexOf(this));
        if(m_visible)
        {
            m_parent->prepareGeometryChanged();
            m_parent->updateChildren();
            m_parent->update();
        }
        m_parent = 0;
    }

    if(m_parentGeometry)
    {
        link(0);
    }

    delete m_geometry;
    delete m_data;
    delete m_pen;

    releaseBuffer();
}

void FwPrimitive::createNewBuffer()
{
    releaseBuffer();
    if(m_geometry->rect().width() != 0 && m_geometry->rect().height() != 0)
    {
        m_buffer = m_scene->view()->createBuffer(m_bufferMode, m_geometry->rect().size());
        if(m_buffer)
        {
            m_buffer->setBlendingEnable(true);
            updateBuffer();
        }
    }
}

void FwPrimitive::releaseBuffer()
{
    if(m_buffer)
    {
        delete m_buffer;
        m_buffer = 0;
    }
}

void FwPrimitive::updateBuffer()
{
    if(m_buffer)
    {
        FwRender* render = m_buffer->createRender();
        if(render)
        {
            FwPainter painter(m_geometry->rect(), m_geometry->rect(), render);
            paint(&painter, m_geometry->rect());
            bufferDirty = false;
        }
    }
}

void FwPrimitive::setBufferMode(Fw::BufferMode mode)
{
    if(m_bufferMode != mode)
    {
        prepareGeometryChanged();
        m_bufferMode = mode;
        releaseBuffer();
        bufferDirty = true;
        update();
    }
}

void FwPrimitive::update(bool needUpdateBuffer)
{
    if(_startChanged > 0 && ((--_startChanged) == 0))
    {
        if(m_geometry->isDirty())
        {
            m_boundingRect = m_geometry->rect();
            boundingRectChangedEvent(m_boundingRect);

            if(m_drawer)
            {
                m_drawer->setPrimitiveRect(m_geometry->rect());
            }

            if(m_bufferMode)
            {
                if(m_buffer && m_geometry->sizeChanged())
                {
                    releaseBuffer();
                    bufferDirty = true;
                }
                else
                {
                    bufferDirty = (!m_buffer || needUpdateBuffer);
                }
            }

            m_geometry->apply();

            if(m_parent)
            {
                m_contentDirty = true;
                m_parent->childSizeChanged = m_parent->childSizeChanged || m_geometry->sizeChanged();
                m_parent->updateChildrenRect();
            }
            else
            {
                invalidateCanvas(rect());
            }
        }
    }
}

void FwPrimitive::setZIndex(int zIndex)
{
    if(m_zIndex != zIndex)
    {
        m_zIndex = zIndex;
        if(m_parent)
        {
            m_parent->sortZIndex();
        }
    }
}

void FwPrimitive::setVisible(bool visible)
{
    if(m_visible != visible)
    {
        m_visible = visible;
        if(m_parent)
        {
            bool oldVisibleOnScreen = visibleOnScreen;
            if(oldVisibleOnScreen != (visibleOnScreen = m_visible && m_parent->visibleOnScreen))
            {
                prepareGeometryChanged();
                m_contentDirty = true;
                m_parent->updateChildrenRect();
                update();
            }
        }
        visibleChangedEvent();
    }
}

void FwPrimitive::prepareGeometryChanged()
{
    if(_startChanged == 0)
    {
        m_contentDirty = true;
        invalidateCanvas(m_parent ? m_parent->m_childrenRect : rect());
    }
    ++_startChanged;
}

void FwPrimitive::visibleChangedEvent()
{
}

void FwPrimitive::apply(FwMLObject* object)
{
    prepareGeometryChanged();

    QRect rect = this->rect();
    if(loadRect(object, &rect))
    {
        setRect(rect);
    }

    FwMLNode* zIndexNode = object->attribute("zindex");
    if(zIndexNode)
    {
        bool bOk = false;
        int zindex = zIndexNode->toInt(&bOk);
        if(bOk)
        {
            setZIndex(zindex);
        }
    }

    FwMLObject* drawerObject = object->attribute("drawer")->cast<FwMLObject>();
    if(drawerObject)
    {
        FwMLString* drawerNameNode = drawerObject->attribute("name")->cast<FwMLString>();
        if(drawerNameNode && !drawerNameNode->value().isEmpty())
        {
            delete  m_drawer;
            m_drawer = createDrawer(drawerNameNode->value(), drawerObject);
        }
    }

    if(!m_drawer)
    {
        FwPen* pen = createPen(object, "pen");
        if(pen)
        {
             setPen(pen);
        }
    }


    FwMLObject* marginObject = object->attribute("margin")->cast<FwMLObject>();
    if(marginObject)
    {
        FwMargin margin;
        if(margin.apply(marginObject))
        {
            m_geometry->setMargin(margin);
            updateGeometryRect();
        }
    }

    FwMLNode* ignoreParentMargin = object->attribute("ignoreParentMargin");
    if(ignoreParentMargin)
    {
        bool bOk = false;
        bool enable = ignoreParentMargin->toBool(&bOk);
        if(bOk)
        {
            setIgnoreParentMargin(enable);
        }
    }

    update();
}

FwPixmap FwPrimitive::createPixmap(const QSize& size) const
{
    return m_scene->view()->pixmap(size);
}

FwPixmap FwPrimitive::createPixmap(const FwPixmapDescription& desc) const
{
    return m_scene->view()->pixmap(desc);
}

FwPixmap FwPrimitive::createPixmap(FwMLNode* node) const
{
    FwPixmapDescription desc;
    if(desc.apply(node))
    {
        return createPixmap(desc);
    }
    return FwPixmap();
}

FwFont FwPrimitive::createFont(const FwFontDescription& desc)
{
    return m_scene->view()->font(desc);
}

FwFont FwPrimitive::createFont(FwMLObject* object, const QByteArray& attribute)
{
    FwMLNode* fontNode = object->attribute(attribute);
    if(fontNode)
    {
        FwFontDescription desc;
        if(desc.apply(fontNode))
        {
            return createFont(desc);
        }
    }
    return FwFont();
}

FwPen* FwPrimitive::createPen(FwMLObject* object, const QByteArray& penAttr)
{
    bool bOk = false;

    int width = 1;
    FwColor color(0xFF, 0xFF, 0xFF, 0xFF);
    QPoint shadowPos(0, 0);
    FwColor shadowColor(0xFF, 0xD0, 0xD0, 0xD0);

    FwMLNode* colorNode = 0;
    FwMLNode* shadowNode = 0;
    FwMLNode* shadowColorNode = 0;

    FwMLObject* penNode = object->attribute(penAttr)->cast<FwMLObject>();
    if(penNode)
    {
        FwMLNode* widthNode = penNode->attribute("width");
        if(widthNode)
        {
            width = widthNode->toInt(&bOk);
            if(!bOk)
            {
                return 0;
            }
        }

        colorNode = penNode->attribute("color");
        shadowNode = penNode->attribute("shadow");
    }
    else
    {
        if(!(colorNode = object->attribute("color")))
        {
            return 0;
        }
        shadowNode = object->attribute("shadow");
    }

    if(colorNode)
    {
        color = colorNode->toColor(&bOk);
        if(!bOk)
        {
            return 0;
        }
    }

    if(shadowNode)
    {
        FwMLObject* shadowObject = shadowNode->cast<FwMLObject>();
        if(shadowObject)
        {
            if(!loadPos(shadowObject, &shadowPos))
            {
                return false;
            }
            shadowColorNode = shadowObject->attribute("color");
        }
        else
        {
            shadowPos.setX(2);
            shadowPos.setY(2);
            shadowColorNode = shadowNode;
        }
    }

    if(shadowColorNode)
    {
        shadowColor = shadowColorNode->toColor(&bOk);
        if(!bOk)
        {
            return 0;
        }
    }

    FwPen* pen = new FwPen(width, color);
    if(shadowPos.x() != 0 || shadowPos.y() != 0)
    {
        pen->setShadow(new FwShadow(shadowPos, shadowColor));
    }

    return pen;
}

FwBrush* FwPrimitive::createBrush(FwMLObject* object)
{
    FwBrush* brush = 0;

    FwMLNode* background = object->attribute("background");
    if(background)
    {
        FwPixmap px = createPixmap(background);
        if(!px.isNull())
        {
            brush = new FwBrushTexture(this, px);
        }
    }
    else
    {
        FwMLNode* bgColorNode = object->attribute("bgcolor");
        if(bgColorNode)
        {
            bool bOk = false;
            FwColor bgColor = bgColorNode->toColor(&bOk);
            if(bOk)
            {
                brush = new FwBrushSolid(this, bgColor);
            }
        }
    }

    if(brush)
    {
        brush->setBorder(createPen(object, "border"));
    }

    return brush;
}

void FwPrimitive::setPosition(Fw::HorizontalPosition hPosition, Fw::VerticalPosition vPosition)
{
    if(m_hPosition != hPosition || m_vPosition != vPosition)
    {
        m_hPosition = hPosition;
        m_vPosition = vPosition;
        if(m_parentGeometry)
        {
            updateGeometryRect();
        }
    }
}

void FwPrimitive::updateGeometryRect()
{
    QRect currentRect(m_pos, m_size);

    if(m_parentGeometry)
    {
        const QRect& parentRect = this->parentRect();
        switch(m_hPosition)
        {
        case Fw::HP_Unchanged:
            currentRect.moveLeft(parentRect.x() + m_pos.x());
            break;

        case Fw::HP_BeforeLeft:
            currentRect.moveLeft(parentRect.x() - currentRect.width() + m_pos.x());
            break;

        case Fw::HP_Left:
            currentRect.moveLeft(parentRect.x() + m_pos.x());
            break;

        case Fw::HP_Center:
            currentRect.moveLeft(parentRect.x() + (parentRect.width() - currentRect.width()) * 0.5 + m_pos.x());
            break;

        case Fw::HP_CenterDock:
            currentRect.moveLeft(parentRect.x() + m_pos.x());
            currentRect.setWidth(parentRect.width());
            break;

        case Fw::HP_Right:
            currentRect.moveLeft(parentRect.right() + 1 - currentRect.width() + m_pos.x());
            break;

        case Fw::HP_AfterRight:
            currentRect.moveLeft(parentRect.right() + m_pos.x());
            break;

        default:
            break;
        }

        switch(m_vPosition)
        {
        case Fw::VP_Unchanged:
            currentRect.moveTop(parentRect.y() + m_pos.y());
            break;

        case Fw::VP_BeforeTop:
            currentRect.moveTop(parentRect.y() - currentRect.height() + m_pos.y());
            break;

        case Fw::VP_Top:
            currentRect.moveTop(parentRect.y() + m_pos.y());
            break;

        case Fw::VP_Middle:
            currentRect.moveTop(parentRect.y() + (parentRect.height() - currentRect.height()) * 0.5 + m_pos.y());
            break;

        case Fw::VP_MiddleDock:
            currentRect.moveTop(parentRect.y()+ m_pos.y());
            currentRect.setHeight(parentRect.height());
            break;

        case Fw::VP_Bottom:
            currentRect.moveTop(parentRect.bottom() - currentRect.height() + m_pos.y());
            break;

        case Fw::VP_AfterBottom:
            currentRect.moveTop(parentRect.bottom() + m_pos.y());
            break;

        default:
            break;
        }
    }

    if(m_geometry->rect() != currentRect)
    {
        prepareGeometryChanged();
        geometryChangedEvent(m_geometry->rect(), currentRect);
        m_geometry->setRect(currentRect);
        update();
    }
}

void FwPrimitive::link(FwGeometry* parentGeometry)
{
    if(m_parentGeometry != parentGeometry)
    {
        if(m_parentGeometry)
        {
            Q_ASSERT(m_parentGeometry->contains(this));

            int array_size = m_parentGeometry->anchors.size();
            if(array_size > 1)
            {
                QVarLengthArray<FwPrimitive*> newAnchors(array_size - 1);
                FwPrimitive** iterAnchor = m_parentGeometry->anchors.data();
                for(int i = 0, j = 0; i < array_size; i++, iterAnchor++)
                {
                    if((*iterAnchor) != this)
                    {
                        newAnchors[j] = (*iterAnchor);
                        j++;
                    }
                }
                m_parentGeometry->anchors = newAnchors;
            }
            else
            {
                m_parentGeometry->anchors.clear();
            }

            m_parentGeometry = 0;
        }

        if(parentGeometry)
        {
            if(m_parent)
            {
                m_ignoreParentMargin = (m_parent->geometry() != parentGeometry);
            }
            else
            {
                m_ignoreParentMargin = true;
            }
            m_parentGeometry = parentGeometry;
            m_parentGeometry->anchors.append(this);
            updateGeometryRect();
        }
    }
}

void FwPrimitive::setPos(const QPoint& pos)
{
    if(m_pos != pos)
    {
        m_pos = pos;
        updateGeometryRect();
    }
}

void FwPrimitive::setSize(const QSize& size)
{
    if(m_size != size)
    {
        m_size = size;
        updateGeometryRect();
    }
}

void FwPrimitive::setRect(const QRect& rect)
{
    if(rect != this->rect())
    {
        m_pos = rect.topLeft();
        m_size = rect.size();
        updateGeometryRect();
    }
}

bool FwPrimitive::loadPos(FwMLObject* object, QPoint* pos)
{
    bool bOk = false;
    int x = pos->x();
    int y = pos->y();
    FwMLObject* posObject = object;

    FwMLNode* posNode = object->attribute("pos");
    if(posNode)
    {
        posObject = posNode->cast<FwMLObject>();
        if(!posObject)
        {
            return false;
        }
    }

    FwMLNode* xNode = posObject->attribute("x");
    if(xNode)
    {
        x = xNode->toInt(&bOk);
        if(!bOk)
        {
            return false;
        }
    }
    FwMLNode* yNode = posObject->attribute("y");
    if(yNode)
    {
        y = yNode->toInt(&bOk);
        if(!bOk)
        {
            return false;
        }
    }

    pos->setX(x);
    pos->setY(y);
    return true;
}

bool FwPrimitive::loadSize(FwMLObject* object,QSize* size)
{
    bool bOk = false;
    int width = size->width();
    int height = size->height();
    FwMLObject* sizeObject = object;

    FwMLNode* sizeNode = object->attribute("size");
    if(sizeNode)
    {
        sizeObject = sizeNode->cast<FwMLObject>();
        if(!sizeObject)
        {
            return false;
        }
    }

    FwMLNode* widthNode = sizeObject->attribute("width");
    if(widthNode)
    {
        width = widthNode->toInt(&bOk);
        if(!bOk || width < 0)
        {
            return false;
        }
    }
    FwMLNode* heightNode = sizeObject->attribute("height");
    if(heightNode)
    {
        height = heightNode->toInt(&bOk);
        if(!bOk || height < 0)
        {
            return false;
        }
    }

    size->setWidth(width);
    size->setHeight(height);
    return true;
}

bool FwPrimitive::loadRect(FwMLObject* object, QRect* rect)
{
    FwMLNode* rectNode = object->attribute("rect");
    if(rectNode)
    {
        FwMLObject* rectObject = rectNode->cast<FwMLObject>();
        if(!rectObject)
        {
            return false;
        }

        FwMLNode* node = rectObject->attribute("x");
        if(node)
        {
            bool bOk = false;
            int x = node->toInt(&bOk);
            if(bOk && (node = rectObject->attribute("y")))
            {
                int y = node->toInt(&bOk);
                if(bOk && (node = rectObject->attribute("width")))
                {
                    int width = node->toInt(&bOk);
                    if(bOk && (node = rectObject->attribute("height")))
                    {
                        int height = node->toInt(&bOk);
                        if(bOk)
                        {
                            rect->setRect(x, y, width, height);
                            return true;
                        }
                    }
                }
            }
        }
        else
        {
            FwMLNode* node = rectObject->attribute("x1");
            if(node)
            {
                bool bOk = false;
                int x1 = node->toInt(&bOk);
                if(bOk && (node = rectObject->attribute("y1")))
                {
                    int y1 = node->toInt(&bOk);
                    if(bOk && (node = rectObject->attribute("x2")))
                    {
                        int x2 = node->toInt(&bOk);
                        if(bOk && (node = rectObject->attribute("y2")))
                        {
                            int y2 = node->toInt(&bOk);
                            if(bOk)
                            {
                                rect->setCoords(x1, y1, x2, y2);
                                return true;
                            }
                        }
                    }
                }
            }
        }

        return false;

    }

    QPoint pos = rect->topLeft();
    QSize size = rect->size();
    if(!loadPos(object, &pos))
    {
        return false;
    }
    if(!loadSize(object, &size))
    {
        return false;
    }
    rect->setRect(pos.x(), pos.y(), size.width(), size.height());
    return true;
}

void FwPrimitive::geometryChangedEvent(const QRect& oldRect, QRect& rect)
{
    Q_UNUSED(oldRect);
    Q_UNUSED(rect);
}

void FwPrimitive::boundingRectChangedEvent(QRect& boundingRect)
{
    Q_UNUSED(boundingRect);
}

void FwPrimitive::setPen(FwPen* pen)
{
    if(m_pen != pen)
    {
        prepareGeometryChanged();

        delete m_pen;
        m_pen = pen;
        penChangedEvent(pen);

        update();
    }
}

void FwPrimitive::setPenColor(const FwColor& color)
{
    if(!m_pen)
    {
        setPen(new FwPen(1, color));
        return;
    }
    else if(m_pen->color() != color)
    {
        prepareGeometryChanged();
        m_pen->setColor(color);
        penChangedEvent(m_pen);
        update();
    }
}

void FwPrimitive::penChangedEvent(FwPen* pen)
{
    Q_UNUSED(pen);
}

void FwPrimitive::invalidateCanvas(const QRect& clipRect)
{
    if(m_contentDirty && m_scene->isActive())
    {
         m_scene->view()->update(clipRect.intersected(m_boundingRect));
         m_contentDirty = false;
    }
}

void FwPrimitive::setIgnoreParentMargin(bool enable)
{
    if(m_ignoreParentMargin != enable)
    {
        m_ignoreParentMargin = enable;
        updateGeometryRect();
    }
}

FwDrawer* FwPrimitive::createDrawer(const QByteArray& name, FwMLObject* object) const
{
    Q_UNUSED(object);
    return 0;
}
