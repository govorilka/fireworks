#include <QtCore/qdebug.h>

#include "fwprimitive.h"
#include "fwprimitivegroup.h"

#include "fwgui/fwscene.h"
#include "fwgui/fwcanvas.h"
#include "fwgui/fwwidget.h"
#include "fwgui/fwgraphicsview.h"
#include "fwgui/fwpixmap.h"

#include "fwcore/fwml.h"

FwPrimitive::FwPrimitive(FwPrimitiveGroup* parent) :
    m_parent(parent),
    m_scene(0),

    m_x(0),
    m_y(0),

    m_anchor(0),
    m_geometry(new FwGeometry()),

    m_boundingRect(0, 0, 0, 0),
    m_boundingRectDirty(false),

    m_bufferMode(Fw::BM_NoBuffer),
    bufferDirty(false),
    m_buffer(0),

    m_visible(true),
    visibleOnScreen(false),

    m_zIndex(0),

    _startChanged(0)
{
    if(m_parent)
    {
        visibleOnScreen = m_parent->visibleOnScreen;
        m_scene = m_parent->m_scene;
        m_parent->prepareGeometryChanged();
        m_parent->m_items.append(this);
        m_parent->sortZIndex();
        m_parent->update();
        setPosition(Fw::P_Relative);
    }
}

FwPrimitive::~FwPrimitive()
{
    if(m_parent)
    {
        m_parent->m_items.remove(m_parent->m_items.indexOf(this));
        if(m_visible)
        {
            m_parent->prepareGeometryChanged();
            m_parent->m_boundingRectDirty = true;
            m_parent->update();
        }
        m_parent = 0;
    }
    if(m_anchor)
    {
        delete m_anchor;
        m_anchor = 0;
    }
    if(m_geometry)
    {
        delete m_geometry;
        m_geometry = 0;
    }
    releaseBuffer();
}

Fw::Position FwPrimitive::position() const
{
    if(m_anchor && m_parent)
    {
        return m_anchor->geometry() == m_parent->m_geometry
                    ? Fw::P_Relative : Fw::P_Absolute;
    }
    return Fw::P_Fixed;
}

void FwPrimitive::setPosition(Fw::Position position)
{
    if(this->position() != position)
    {
        if(position == Fw::P_Fixed)
        {
            if(m_anchor)
            {
                delete m_anchor;
                m_anchor = 0;
            }
        }
        else
        {
            if(!m_anchor)
            {
                m_anchor = new FwAnchor(this);
                m_anchor->setPolicy(FwAnchor::HP_Left, FwAnchor::VP_Top);
            }
            if(position == Fw::P_Relative && m_parent)
            {
                m_parent->linkAnchor(m_anchor);
            }
        }
    }
}

void FwPrimitive::createNewBuffer()
{
    releaseBuffer();
    if(m_boundingRect.width() != 0 && m_boundingRect.height() != 0)
    {
        m_buffer = m_scene->view()->createBuffer(m_bufferMode, m_boundingRect.size());
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
            FwCanvas canvas(render);
            canvas.setNullPos(-m_boundingRect.topLeft());
            canvas.clear();
            paint(&canvas);
            bufferDirty = false;
            //canvas.flip(m_boundingRect);
        }
    }
}

void FwPrimitive::setBufferMode(Fw::BufferMode mode)
{
    if(m_bufferMode != mode)
    {
        m_bufferMode = mode;
        releaseBuffer();
        bufferDirty = true;
        invalidate();
    }
}

void FwPrimitive::setGeometryRect(const QRect& rect)
{
    if(m_geometry->rect() != rect)
    {
        prepareGeometryChanged();
        m_geometry->setRect(rect);
        update(false);
    }
}

void FwPrimitive::update(bool needUpdateBuffer)
{
    if(_startChanged > 0 && ((--_startChanged) == 0))
    {        
        QRect oldBoundingRect = m_boundingRect;

        m_boundingRect = updateGeometry(m_geometry->rect());
        m_boundingRectDirty = false;

        if(oldBoundingRect != m_boundingRect)
        {
            if(m_bufferMode)
            {
                if(m_buffer && (oldBoundingRect.size() != m_boundingRect.size()))
                {
                    releaseBuffer();
                    bufferDirty = true;
                }
                else
                {
                    bufferDirty = (!m_buffer || needUpdateBuffer);
                }
            }

            FwPrimitiveGroup* parent = m_parent;
            while(parent)
            {
                parent->m_boundingRectDirty = true;
                parent = parent->m_parent;
            }
        }
        invalidate();
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
        visibleOnScreen = m_parent ? (m_visible && m_parent->visibleOnScreen) : m_visible;
        visibleChangedEvent();
        if(_startChanged == 0)
        {
            m_scene->update(m_boundingRect);
        }
    }
}

void FwPrimitive::invalidate()
{
    if(_startChanged == 0 && visibleOnScreen)
    {
        m_scene->update(m_boundingRect);
    }
}

void FwPrimitive::visibleChangedEvent()
{
}

QRect FwPrimitive::updateGeometry(const QRect& rect)
{
    return rect;
}

void FwPrimitive::paint(FwCanvas* canvas)
{
}

void FwPrimitive::apply(FwMLObject* object)
{
    prepareGeometryChanged();

    FwMLObject* rectObject = object->attribute("rect")->cast<FwMLObject>();
    if(rectObject)
    {
        bool bOk = false;
        QRect rect = rectObject->toRect(&bOk);
        if(bOk)
        {
            setRect(rect);
        }
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

    update();
}

FwPixmap FwPrimitive::createPixmap(const QSize& size)
{
    return m_scene->view()->pixmap(size);
}

FwPixmap FwPrimitive::createPixmap(const FwPixmapDescription& desc)
{
    return m_scene->view()->pixmap(desc);
}

FwPixmap FwPrimitive::createPixmap(FwMLNode* node)
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

FwPenPtr FwPrimitive::createPen(FwMLObject* object, const QByteArray& attribute)
{
    FwMLNode* colorNode = object->attribute(attribute);
    if(colorNode)
    {
        bool bOk = false;
        FwColor color = colorNode->toColor(&bOk);
        if(bOk)
        {
            return FwPenPtr(new FwPen(1, color));
        }
    }

    return FwPenPtr();
}
