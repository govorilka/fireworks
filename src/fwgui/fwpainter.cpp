#include <QtCore/qdebug.h>

#include "fwpainter.h"

FwPainter::FwPainter(const QRect& window, FwRender* render) :
    m_nullX(window.x()),
    m_nullY(window.y()),
    m_windowSize(window.size()),
    m_render(render),
    m_clipRect(0, 0, 0, 0),
    m_color(0x00, 0x00, 0x00, 0x00),
    m_opacity(0)
{
}

FwPainter::~FwPainter()
{
    if(m_render)
    {
        delete m_render;
    }
}
