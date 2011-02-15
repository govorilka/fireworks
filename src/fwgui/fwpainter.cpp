#include "fwpainter.h"

FwPainter::FwPainter(const QRect& window, const QRect& clipRect, FwRender* render) :
    m_nullX(window.x()),
    m_nullY(window.y()),
    m_windowSize(window.size()),
    m_render(render),
    m_clipRect(QRect(QPoint(0, 0), window.size())),
    m_color(0x00, 0x00, 0x00, 0x00),
    m_opacity(1.0)
{
    setClipRect(clipRect);
    setColor(0xFF, 0x00, 0x00, 0x00);
}

FwPainter::~FwPainter()
{
    if(m_render)
    {
        delete m_render;
    }
}

QRect FwPainter::setClipRect(const QRect& rect)
{
    QRect oldRect = m_clipRect;

    QRect clipRect = rect.translated(m_nullX, m_nullY);
    if(m_clipRect != clipRect)
    {
        m_clipRect = clipRect;
        restoreClipRect(m_clipRect);
    }

    return oldRect;
}

void FwPainter::restoreClipRect(const QRect& rect)
{
    if(m_clipRect != QRect(QPoint(0, 0), m_windowSize))
    {
        m_render->setClipRect(m_clipRect);
    }
    else
    {
        m_render->resetClipRect();
    }
}
