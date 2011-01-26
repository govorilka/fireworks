#include <QtCore/qdebug.h>

#include "fwbrush.h"
#include "fwpainter.h"

FwBrush::FwBrush()
{
}

FwBrush::~FwBrush()
{
}

void FwBrush::drawRect(FwPainter* painter, const QRect& clipRect)
{
    drawBackground(painter, m_border ? m_border->drawStroke(painter, clipRect) : clipRect);
}

void FwBrush::setSourceRect(const QRect& rect)
{
    Q_UNUSED(rect);
}

QRect FwBrush::sourceRect() const
{
    return QRect();
}

///////////////////////////////////////////////////////////////////////////////

FwBrushSolid::FwBrushSolid(const FwColor& color) :
    BaseClass(),
    m_color(color)
{
}

void FwBrushSolid::drawBackground(FwPainter* painter, const QRect& clipRect)
{
    painter->setColor(m_color);
    painter->drawFillRect(clipRect);
}

///////////////////////////////////////////////////////////////////////////////

FwBrushTexture::FwBrushTexture(const FwPixmap& pixmap) :
    BaseClass(),
    m_pixmap(pixmap),
    m_sourceRect(0, 0, 0, 0)
{
    m_displayPixmap = pixmap;
}

void FwBrushTexture::drawBackground(FwPainter* painter, const QRect& clipRect)
{
    painter->drawPixmap(clipRect,
                        m_displayPixmap,
                        &clipRect.translated(m_sourceRect.topLeft()));
}

void FwBrushTexture::setSourceRect(const QRect& rect)
{
    if(rect != m_sourceRect && !m_pixmap.isNull())
    {
        if(rect.size() != m_sourceRect.size())
        {
            if(!m_pixmap.isNull() && rect.size().isValid())
            {
                if(rect.size() != m_pixmap.size())
                {
                    m_displayPixmap = m_pixmap.resized(rect.size(), FwPixmap::RM_Scale);
                }
                else
                {
                    m_displayPixmap = m_pixmap;
                }
            }
            else
            {
                m_displayPixmap = FwPixmap();
            }
        }
        m_sourceRect = rect;
    }
}

QRect FwBrushTexture::sourceRect() const
{
    return m_sourceRect;
}
