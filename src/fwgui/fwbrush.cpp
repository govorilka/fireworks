#include <QtCore/qdebug.h>

#include "fwbrush.h"
#include "fwpainter.h"

FwBrush::FwBrush(FwPrimitive* primitive) :
    m_sourceRect(0, 0, 0, 0),
    m_backgroundRect(0, 0, 0, 0),
    m_border(0),
    m_primitive(primitive)
{
}

FwBrush::~FwBrush()
{
    if(m_border)
    {
        delete m_border;
    }
}

void FwBrush::drawRect(FwPainter* painter, const QRect& clipRect)
{
    drawBackground(painter, m_backgroundRect.intersected(clipRect));
    if(m_border)
    {
       m_border->drawStroke(painter, m_sourceRect);
    }
}

void FwBrush::setBorder(FwPen* border)
{
    if(m_border != border)
    {
        if(m_border)
        {
            delete m_border;
        }
        m_border = border;
        updateGeometry();
    }
    m_border = border;
}

void FwBrush::setSourceRect(const QRect& rect)
{
    if(m_sourceRect != rect)
    {
        m_sourceRect = rect;
        updateGeometry();
    }
}

///////////////////////////////////////////////////////////////////////////////

FwBrushSolid::FwBrushSolid(FwPrimitive* primitive, const FwColor& color) :
    BaseClass(primitive),
    m_color(color)
{
}

void FwBrushSolid::drawBackground(FwPainter* painter, const QRect& clipRect)
{
    painter->setColor(m_color);
    painter->drawFillRect(clipRect);
}

void FwBrushSolid::updateSourceRect(const QRect& rect)
{
    Q_UNUSED(rect);
}

///////////////////////////////////////////////////////////////////////////////

FwBrushTexture::FwBrushTexture(FwPrimitive* primitive, const FwPixmap& pixmap) :
    BaseClass(primitive),
    m_pixmap(pixmap)
{
    m_displayPixmap = pixmap;
}

void FwBrushTexture::drawBackground(FwPainter* painter, const QRect& clipRect)
{
    painter->drawPixmap(clipRect,
                        m_displayPixmap,
                        &clipRect.translated(-sourceRect().topLeft()));
}

void FwBrushTexture::updateSourceRect(const QRect& rect)
{
    if(!m_pixmap.isNull() && rect.size().isValid())
    {
        m_displayPixmap = (rect.size() != m_pixmap.size()) ? m_pixmap.resized(rect.size(), FwPixmap::RM_Scale) : m_pixmap;
    }
    else
    {
        m_displayPixmap = FwPixmap();
    }
}
