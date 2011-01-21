#include <QtCore/qdebug.h>

#include "fwbrush.h"
#include "fwcanvas.h"

FwBrush::FwBrush()
{
}

FwBrush::~FwBrush()
{
}

void FwBrush::drawRect(FwCanvas* canvas, const QRect& rect)
{
    if(m_border)
    {
        drawBackground(canvas, m_border->drawStroke(canvas, rect));
    }
    else
    {
        drawBackground(canvas, rect);
    }
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

void FwBrushSolid::drawBackground(FwCanvas* canvas, const QRect& rect)
{
    canvas->setColor(m_color);
    canvas->drawFillRect(rect);
}

///////////////////////////////////////////////////////////////////////////////

FwBrushTexture::FwBrushTexture(const FwPixmap& pixmap) :
    BaseClass(),
    m_pixmap(pixmap),
    m_sourceRect(0, 0, 0, 0)
{
    m_displayPixmap = pixmap;
}

void FwBrushTexture::drawBackground(FwCanvas* canvas, const QRect& rect)
{
    canvas->drawPixmap(rect, m_displayPixmap);
}

void FwBrushTexture::setSourceRect(const QRect& rect)
{
    if(rect != m_sourceRect)
    {
        if(rect.size() != m_sourceRect.size())
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
        m_sourceRect = rect;
    }
}

QRect FwBrushTexture::sourceRect() const
{
    return m_sourceRect;
}
