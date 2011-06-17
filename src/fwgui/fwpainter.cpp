#include <QtCore/qdebug.h>

#include "fwpainter.h"

FwPainter::FwPainter(const QRect& window, const QRect& clipRect, FwRender* render) :
    m_nullX(window.x()),
    m_nullY(window.y()),
    m_windowSize(window.size()),
    m_render(render),
    m_clipRect(0, 0, 0, 0),
    m_color(0x00, 0x00, 0x00, 0x00),
    m_opacity(1.0)
{
    if(window != clipRect)
    {
        setClipRect(clipRect);
    }
    else
    {
        m_clipRect = clipRect;
    }
    setColor(0xFF, 0x00, 0x00, 0x00);
}

FwPainter::~FwPainter()
{
    if(m_render)
    {
        delete m_render;
    }
}
